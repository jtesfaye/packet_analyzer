//
// Created by jeremiah tesfaye on 10/9/25.
//


#include <capture/CaptureSession.h>
#include <util/PacketObserver.h>
#include <capture/PacketCapture.h>
#include <iostream>
#include <filesystem>
#include <layerx/iana_numbers.h>


CaptureSession::CaptureSession(const CaptureConfig &config)
: running(true)
, capture_on(false)
, m_details_cache(std::make_shared<DetailParseCache>(config.packet_count > 0 ? config.packet_count : 100))
, m_pkt_ref_buffer(std::make_shared<InitialParseBuffer>(config.packet_count > 0 ? config.packet_count : 100))
, m_observer(std::make_shared<PacketObserver>(*m_pkt_ref_buffer, *m_details_cache))
, m_handle(nullptr, close_handle)
, m_bpf_program(nullptr, free_bpf_program)
{

    if (config.mode == CaptureMode::Online) {

        initialize_online_handle(
            config.source,
            config.settings,
            config.capture_size,
            config.filter
            );

        int l2 = pcap_dlt_to_ieee.at(pcap_datalink(m_handle.get()));

        std::string temp_file = std::filesystem::temp_directory_path().generic_string() + "foobar.pcap";

        m_initial_parser = std::make_shared<InitialParser>(l2, config.flags);
        m_detail_parser = std::make_shared<DetailParser>();

        EngineInit init {m_initial_parser, m_detail_parser, m_pkt_ref_buffer, m_details_cache, m_observer ,m_raw_pkt_queue};
        m_pool = std::make_shared<ParsingEngine>(init);

        m_pcap_file = std::make_shared<PcapFile> (
            temp_file,
            m_handle.get()
            );


        capture = PacketCapture::createOnlineCapture(
            m_handle.get(),
            config.packet_count,
            config.flags,
            m_pcap_file,
            m_pool,
            m_raw_pkt_queue
            );

    } else if (config.mode == CaptureMode::Offline) {

        initialize_offline_handle(config.source);

        int data_link_type = pcap_datalink(m_handle.get());

        m_pcap_file = std::make_shared<PcapFile>(
            config.source
        );

        m_initial_parser = std::make_shared<InitialParser>(data_link_type, config.flags);
        m_detail_parser = std::make_shared<DetailParser>();

        EngineInit init {m_initial_parser, m_detail_parser, m_pkt_ref_buffer, m_details_cache, m_observer ,m_raw_pkt_queue};
        m_pool = std::make_shared<ParsingEngine>(init);

        capture = PacketCapture::createOfflineCapture(
            m_handle.get(),
            m_pcap_file,
            m_pool,
            m_raw_pkt_queue
            );

    } else {

        throw std::runtime_error("Error initializing capture session");

    }
}

CaptureSession::~CaptureSession() {
    m_pool->shutdown();
    m_observer->set_done();
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

        case CommandType::End:
            running = false;
            break;
    }
}

void CaptureSession::start_capture() {

    capture_on = true;
    std::thread capture_thread { [this] () {
        capture->start_capture();
        if (capture_on)
            capture_on = false;
    }};

    capture_thread.detach();
}

void CaptureSession::stop_capture() const {

    if (capture_on)
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

InitialParseBuffer& CaptureSession::get_buffer() const {
    return *m_pkt_ref_buffer;
}

DetailParseCache &CaptureSession::get_cache() const {
    return *m_details_cache;
}


std::shared_ptr<PacketObserver> CaptureSession::get_observer() const {
    return m_observer;
}






