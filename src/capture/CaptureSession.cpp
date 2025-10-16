//
// Created by jeremiah tesfaye on 10/9/25.
//


#include <capture/CaptureSession.h>
#include <iostream>
#include <capture/PacketCapture.h>
#include <filesystem>

CaptureSession::CaptureSession(const CaptureConfig &config)
: m_handle(nullptr, close_handle)
, running(true)
{

    if (config.mode == CaptureMode::Online) {

        initialize_online_handle(
            config.source,
            config.settings,
            config.capture_size
            );

        std::string temp_file = std::filesystem::temp_directory_path().generic_string() + "foobar.pcap";

        m_pcap_file = std::make_shared<PcapFile> (
            temp_file,
            m_handle.get()
            );

        capture = PacketCapture::createOnlineCapture(m_handle.get(),
            config.packet_count,
            config.flags,
            m_pcap_file
            );

    } else if (config.mode == CaptureMode::Offline) {

        initialize_offline_handle(config.source);

        m_pcap_file = std::make_shared<PcapFile>(
            config.source
        );

        capture = PacketCapture::createOfflineCapture(m_handle.get(), m_pcap_file);

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

        case CommandType::End:
            running = false;
            break;
    }
}


void CaptureSession::start_capture() const {


    std::thread capture_thread { [this] () {
        this->capture->start_capture();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "finished\n";
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
    int capture_size) {


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

std::shared_ptr<PacketRefBuffer> CaptureSession::get_buffer() const {

    return capture->get_buffer();
}

std::shared_ptr<PacketObserver> CaptureSession::get_observer() const {

    return capture->get_observer();
}



