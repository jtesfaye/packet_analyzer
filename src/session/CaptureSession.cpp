//
// Created by jeremiah tesfaye on 10/9/25.
//


#include <session/CaptureSession.h>
#include <capture/PacketCapture.h>
#include <iostream>
#include <filesystem>
#include <session/CaptureEngine.h>

CaptureSession::CaptureSession(IEventSink& sink)
: sink(sink)
, running(true)
, capture_on(false)
, m_handle(nullptr, close_handle)
, m_bpf_program(nullptr, free_bpf_program)
{
    start_session();
}
CaptureSession::~CaptureSession() = default;

void CaptureSession::send_command(SessionCommand&& cmd) {
    {
        std::lock_guard l(lock);
        commands.push(std::move(cmd));
    }
    cv.notify_one();
}

void CaptureSession::start_session() {
    std::thread worker{[this] ()
    {
        while (running) {
            std::unique_lock ul(lock);
            cv.wait(ul, [this] () {
                return !commands.empty();
            });
            auto cmd = std::move(commands.front());
            commands.pop();
            ul.unlock();
            process_cmd(cmd);
        }
    }};

    worker.detach();
}

void CaptureSession::process_cmd(SessionCommand &cmd) {
    switch (cmd.type) {
        case CommandType::Start:
        {
            auto arg = std::get<CaptureConfig>(cmd.arg);
            start_capture(arg);
            break;
        }
        case CommandType::Stop:
            stop_capture();
            break;
        case CommandType::Save:
            save_capture(std::get<std::string>(cmd.arg));
            break;
        case CommandType::End:
            running = false;
            break;
        case CommandType::SendPackets:
        {
            auto data = std::move(std::get<std::vector<packet_data>>(cmd.arg));
            sink.on_packet_batch(data);
            break;
        }
        case CommandType::GetDetails: {
            auto details = m_engine->get_detail(std::get<size_t>(cmd.arg));
            sink.on_detail_request(details);
            break;
        }
        case CommandType::GetStats: {
            auto stats = m_engine->get_stream_stats(std::get<size_t>(cmd.arg));
            sink.on_stream_update(stats);
            break;
        }
    }
}

void CaptureSession::start_capture(const CaptureConfig& config) {
    capture_on = true;
    std::function on_batch = [this](std::vector<packet_data>& data) {
        auto d = SessionCommand::send_packets(std::move(data));
        send_command(std::move(d));
    };
    if (config.mode == CaptureMode::Online) {

        initialize_online_handle(
        config.source,
            config.settings,
            config.capture_size,
            config.filter
        );

        std::string temp_file = std::filesystem::temp_directory_path().generic_string() + "foobar.pcap";
        const int dlt = pcap_datalink(m_handle.get());
        u_int8_t flags = config.flags;

        m_pcap_file = std::make_shared<PcapFile>(
            temp_file,
            m_handle.get()
            );

        std::unique_ptr<PacketCapture> capture = PacketCapture::createOnlineCapture(
            config.packet_count,
            config.flags,
            CaptureInit {m_handle.get(), m_pcap_file, raw_pkt_queue}
        );
        EngineInit init(std::move(capture),dlt, config.flags, 300,  raw_pkt_queue, std::thread::hardware_concurrency());
        m_engine = std::make_unique<CaptureEngine>(init, on_batch);

    } else if (config.mode == CaptureMode::Offline) {
        initialize_offline_handle(config.source);
        int dlt = pcap_datalink(m_handle.get());

        m_pcap_file = std::make_shared<PcapFile>(
            config.source
        );

        auto capture = PacketCapture::createOfflineCapture(
            CaptureInit {m_handle.get(), m_pcap_file, raw_pkt_queue}
        );

        EngineInit init(std::move(capture),dlt, config.flags, 300,  raw_pkt_queue, std::thread::hardware_concurrency());
        m_engine = std::make_unique<CaptureEngine>(init, on_batch);

    } else {
        throw std::runtime_error("Error initializing capture session");
    }

    std::thread capture_thread { [this] () {
        m_engine->start();
        if (capture_on)
            capture_on = false;
    }};
    capture_thread.detach();
}

void CaptureSession::stop_capture() const {
    if (capture_on) {
        m_engine->stop();
    }
}

bool CaptureSession::save_capture(const std::string& path) const {
    return m_pcap_file->save_file(path);
}

void CaptureSession::close_handle(pcap_t *handle) {
     if (!handle) {
         return;
     }
    pcap_close(handle);
}

void CaptureSession::initialize_online_handle(
    const std::string& device,
    u_int8_t settings,
    int capture_size,
    const std::string& filter) {

    using namespace capture;
    char errbuf[PCAP_ERRBUF_SIZE];
    if (pcap_t* pcap_handle = pcap_create(device.c_str(), errbuf)) {
        m_handle = std::unique_ptr<pcap_t, decltype(&pcap_close)> (
          pcap_handle,
          pcap_close
        );
    } else {
        throw std::runtime_error(errbuf);
    }
    if (m_handle == nullptr) {
        std::cerr << "Error in creating handle: " << errbuf << std::endl;
        return; //probably not the best way to handle an error but ill change it later
    }
    if (capture_size == 0) { //decide how much of the packet will be captures
        pcap_set_snaplen(m_handle.get(), DEFAULT_CAPSIZE);
    }
    else {
        pcap_set_snaplen(m_handle.get(), capture_size);
    }
    if (settings & PROMISC) { //promisous mode
        pcap_set_promisc(m_handle.get(), 1);
    }
    if (settings & PRECISION) { //precision of timestamps
        pcap_set_tstamp_type(m_handle.get(), PCAP_TSTAMP_HOST_HIPREC);
    } else {
        pcap_set_tstamp_type(m_handle.get(), PCAP_TSTAMP_HOST);
    }

    pcap_set_immediate_mode(m_handle.get(),1);
    if (pcap_activate(m_handle.get()) != 0) {
        throw std::runtime_error(pcap_geterr(m_handle.get()));
    }
    if (!filter.empty()) {
        apply_filter(device, filter);
    }
}

void CaptureSession::initialize_offline_handle(const std::string& path) {
    char errbuf[PCAP_ERRBUF_SIZE];
    if (pcap_t* pcap_handle = pcap_open_offline(path.c_str(), errbuf)) {
        m_handle = std::unique_ptr<pcap_t, decltype(&close_handle)> (
          pcap_handle,
          pcap_close
        );
    } else {
        throw std::runtime_error(errbuf);
    }
}

void CaptureSession::free_bpf_program(bpf_program *program) {
    pcap_freecode(program);
}

void CaptureSession::apply_filter(const std::string& device_name, const std::string &filter) {
    if (!m_handle) {
        throw std::runtime_error("apply_filter called before handle initialized");
    }
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 net, mask;
    if (pcap_lookupnet(device_name.c_str(), &net, &mask, errbuf) == -1) {
        net = 0;
        mask = 0;
    }
    pcap_compile(m_handle.get(), m_bpf_program.get(), filter.c_str(), 1, mask);
    pcap_setfilter(m_handle.get(), m_bpf_program.get());
}













