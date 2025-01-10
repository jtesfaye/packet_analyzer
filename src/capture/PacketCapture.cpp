
#include "PacketCapture.h"

namespace Capture {

  PacketCapture::PacketCapture
  (
    const char* device_name, 
    int cnt, 
    Options&& opt
  )
  : _device(device_name),
    _packets_to_capture(cnt),
    _options(opt) {

      _handle = pcap_create(_device, errbuf);

      if (_handle == nullptr) {
        std::cerr << "Error in creating handle: " << errbuf << std::endl;
        return; //probably not the best way to handle an error but ill change it later
      }

    }

}