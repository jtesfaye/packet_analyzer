
#include "../../include/capture/PacketCapture.h"
#include "../../include/capture/Online.h"
#include "../../include/capture/Offline.h"
#include <iostream>
#include <stdexcept>
#include <utility>

namespace capture {

  PacketCapture::PacketCapture()
  : _packets_to_capture(0)
  , m_data_link(-1)
  , _handle(nullptr, close_handle)
  , _device_list(nullptr, free_devices) {
  }


  pcap_t* PacketCapture::handle() const {

    return _handle.get();

  }



  pcap_if_t* PacketCapture::devices() const {

    return _device_list.get();

  }



  void PacketCapture::set_data_link(int dlt) {

    m_data_link = dlt;

  }



  int
  PacketCapture::get_datalink() const {
    return m_data_link;
  }



  void
  PacketCapture::set_buffer(const std::shared_ptr<PacketBuffer> &buf)
  {
    _buffer = buf;
  }



  std::shared_ptr<PacketBuffer> PacketCapture::get_buffer() {

    return _buffer;

  }



  std::unique_ptr<PacketCapture>
  PacketCapture::createOnlineCapture
  (
  std::string& device_name,
  int count,
  int capture_size,
  u_int8_t settings,
  u_int8_t flags
  ) {

    auto cap = std::make_unique<Online>(
      std::move(device_name)
      , count
      , capture_size
      , settings
      , flags);

    cap->set_buffer(std::make_shared<PacketBuffer>(nullptr));

    return cap;

  }



  std::unique_ptr<PacketCapture>
  PacketCapture::createOfflineCapture
  (
  std::string &path_name,
  int count
  ) {

    auto cap = std::make_unique<Offline>(
      std::move(path_name)
      , count);

    cap->set_buffer(std::make_shared<PacketBuffer>(nullptr));

    return cap;

  }



  void
  PacketCapture::start_capture() const {

    using namespace parse;

    auto parser = PacketParse(m_data_link, 0);

    auto file_path = "/Users/jt/projects/workspace/personal_projects/sniffer/tests/pcap_files";

    capture_objects ref {
      parser,
      _buffer.get(),
      pcap_dump_open(handle(), file_path)
    };

    auto callback =[]
    (u_char* data, const struct pcap_pkthdr* header, const u_char* packet) {

      const auto obj = reinterpret_cast<capture_objects*>(data);

      packet_ref ref{obj->parser.start_extract(header, packet)};

      obj->buffer->add(ref);

    };

    pcap_loop(handle(), _packets_to_capture, callback, reinterpret_cast<u_char*>(&ref));

  }



  std::vector<std::string>
  PacketCapture::get_devices() {

    pcap_if_t* list = nullptr;

    std::vector<std::string> names;

    char errbuf[256];

    if (pcap_findalldevs(&list, errbuf) == 0) {

      //fix
    } else {
      std::cerr << "Error: " << errbuf << std::endl;
      return {};
    }

    for (pcap_if_t* it = list; it != nullptr; it = it->next) {

      names.emplace_back(it->name);

    }

    return names;
  }



  void 
  PacketCapture::free_devices
  (pcap_if_t* dev_list) {
    if (!dev_list) {
      return;
    } 

    pcap_freealldevs(dev_list);

  }



  void 
  PacketCapture::close_handle
  (pcap_t* handle) {
    if (!handle) 
      return;

    pcap_close(handle);

  }



  PacketCapture::~PacketCapture() = default;

}