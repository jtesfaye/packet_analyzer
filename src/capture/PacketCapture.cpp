
#include "PacketCapture.h"

namespace capture {

  PacketCapture::PacketCapture
  (
    std::string device_name, 
    int cnt, 
    u_int8_t settings
  )
  : _device(device_name),
    _packets_to_capture(cnt),
    _settings(settings),
    _handle(nullptr, pcap_close),
    _device_list(nullptr, pcap_freealldevs)
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

  PacketCapture::~PacketCapture() {};

}