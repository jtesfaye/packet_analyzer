//
// Created by jeremiah tesfaye on 10/9/25.
//


#include <capture/CaptureSession.h>
#include <iostream>
#include <capture/PacketCapture.h>
#include <filesystem>

CaptureSession::CaptureSession(const CaptureConfig &config)
: m_handle(nullptr, close_handle)
, m_bpf_program(nullptr, free_bpf_program)
, running(true)
, m_buffer(std::make_shared<PacketBuffer<packet_ref>>(20))
, m_observer(std::make_shared<PacketObserver>(*m_buffer))
{

    m_cache = std::make_shared<LRUCache<ProtocolDetails>>(config.packet_count > 0 ? config.packet_count : 100);

    if (config.mode == CaptureMode::Online) {

        initialize_online_handle(
            config.source,
            config.settings,
            config.capture_size,
            config.filter
            );

        int data_link_type = pcap_datalink(m_handle.get());

        std::string temp_file = std::filesystem::temp_directory_path().generic_string() + "foobar.pcap";

        m_initial_parser = std::make_shared<InitialParser>(data_link_type, config.flags);
        m_detail_parser = std::make_shared<DetailParser>(m_buffer, m_cache, m_observer);

        m_pcap_file = std::make_shared<PcapFile> (
            temp_file,
            m_handle.get()
            );

        PoolInit init {m_initial_parser, m_detail_parser, m_buffer};
        m_pool = std::make_shared<ThreadPool>(init);

        capture = PacketCapture::createOnlineCapture(
            m_handle.get(),
            data_link_type,
            config.packet_count,
            config.flags,
            m_pcap_file,
            m_pool
            );

    } else if (config.mode == CaptureMode::Offline) {

        initialize_offline_handle(config.source);

        int data_link_type = pcap_datalink(m_handle.get());

        m_pcap_file = std::make_shared<PcapFile>(
            config.source
        );

        m_initial_parser = std::make_shared<InitialParser>(data_link_type, config.flags);
        m_detail_parser = std::make_shared<DetailParser>(m_buffer, m_cache, m_observer);

        PoolInit init {m_initial_parser, m_detail_parser, m_buffer};
        m_pool = std::make_shared<ThreadPool>(init, 1);

        capture = PacketCapture::createOfflineCapture(
            m_handle.get(),
            data_link_type,
            m_pcap_file,
            m_pool
            );

    } else {

        throw std::runtime_error("Error initializing capture session");

    }
}

void CaptureSession::send_command(const SessionCommand &cmd) {

    {
        std::lock_guard l(lock);
        commands.push(cmd);
    }

    cv.notify_one();
}


void CaptureSession::start_session() {

    while (running) {

        std::unique_lock ul(lock);
        cv.wait(ul, [this] (){return !commands.empty();} );

        auto cmd = std::move(commands.front());
        commands.pop();
        ul.unlock();

        process_cmd(cmd);
    }
}


void CaptureSession::process_cmd(const SessionCommand &cmd) {

    switch (cmd.type) {

        case CommandType::Start:
            start_capture();
            break;

        case CommandType::Stop:
            stop_capture();
            break;

        case CommandType::Save:
            save_capture(std::get<std::string>(cmd.cmd_data));
            break;

        case CommandType::GetDetails:
            get_details(std::get<int>(cmd.cmd_data));
            break;

        case CommandType::End:
            running = false;
            break;
    }
}


void CaptureSession::start_capture() const {


    std::thread capture_thread { [this] () {
        this->capture->start_capture();
    }};

    capture_thread.detach();
}

void CaptureSession::stop_capture() const {

    capture->stop_capture();

}

bool CaptureSession::save_capture(const std::string& path) const {

    return m_pcap_file->save_file(path);

}


void CaptureSession::close_handle(pcap_t *handle) {

    if (!handle)
        return;

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

    //decide how much of the packet will be captures
    if (capture_size == 0)

        pcap_set_snaplen(m_handle.get(), DEFAULT);
    else
        pcap_set_snaplen(m_handle.get(), capture_size);

    //promisous mode
    if (settings & PROMISC) {

        pcap_set_promisc(m_handle.get(), 1);

    }

    //precision of timestamps
    if (settings & PRECISION) {

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

std::shared_ptr<IContainerType<packet_ref>> CaptureSession::get_buffer() const {

    return m_buffer;
}

std::shared_ptr<PacketObserver> CaptureSession::get_observer() const {

    return m_observer;
}






