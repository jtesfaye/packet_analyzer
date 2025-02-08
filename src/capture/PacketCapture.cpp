
#include "PacketCapture.h"
#include <iostream>
#include <stdexcept>

namespace capture {

  PacketCapture::PacketCapture
  (
    std::string device_name, 
    int cnt, 
    u_int8_t settings,
    u_int8_t flags
  )
  : _device(device_name),
    _file_path(""),
    _packets_to_capture(cnt),
    _settings(settings),
    _handle(nullptr, close_handle),
    _device_list(nullptr, free_devices)
  {

    if (pcap_t* pcap_handle = pcap_create(_device.c_str(), errbuf)) {

      _handle = std::unique_ptr<pcap_t, decltype(&pcap_close)> (
        pcap_handle,
        pcap_close
      );

    } else {
      
      throw std::runtime_error(errbuf);

    }

  }

  

  PacketCapture::PacketCapture
  (
    std::string path_name,
    int cnt,
    u_int8_t flags
  )
  : _device{""},
    _file_path{path_name},
    _packets_to_capture{cnt},
    _settings{0},
    _handle{nullptr, close_handle},
    _device_list{nullptr, free_devices} 
  {

    if (pcap_t* pcap_handle = pcap_open_offline(_file_path.c_str(), errbuf)) {

      _handle = std::unique_ptr<pcap_t, decltype(&close_handle)> (
        pcap_handle,
        pcap_close
      );

    } else {

      throw std::runtime_error(errbuf);
      
    }

  }



  void 
  PacketCapture::process_packet
  (const struct pcap_pkthdr *head, const u_char *bytes) {
  
    try {
      _packet_data.set_meta_data(head);
      _packet_data.set_raw_data(bytes);
      
      packet_model model {_packet_data.start_extract()};

      model.l2_display(model.l2);
    } catch (std::out_of_range &e) {
      if (_settings) {
        std::cerr << "Error in online capture: " << e.what();
      } else {
        std::cerr << "Error in offline capture: " << e.what();
      }

    }


  }



  void
  PacketCapture::start_capture() {

    auto 
    callback = []
    (u_char* data, const struct pcap_pkthdr* header, const u_char* packet) {

      PacketCapture* self = reinterpret_cast<PacketCapture*>(data);
      self->process_packet(header, packet);

    };

    std::cout << "capturing link types of: " << pcap_datalink_val_to_name(pcap_datalink(handle())) << "\n";

    pcap_loop(handle(), _packets_to_capture, callback, reinterpret_cast<u_char*>(this));

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

  PacketCapture::~PacketCapture() {};

}