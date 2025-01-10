
#include "Online.h"


namespace Capture {

  OnlinePacketCapture::OnlinePacketCapture
  (
    const char* device_name,
    int count, 
    const Options& options
  ) 
  : PacketCapture(device_name, count, std::move(options)) {

    _handle = pcap_create(_device, errbuf);

    if (_handle == nullptr) {
      std::cerr << "Error in creating handle: " << errbuf << std::endl;
      return; //probably not the best way to handle an error but ill change it later
    }

    //decide how much of the packet will be captures
    if (_options.full_capture) {

      pcap_set_snaplen(_handle, FULL);
      
    } else if (_options.basic_capture == true) {

      pcap_set_snaplen(_handle, BASIC);

    } else {

      pcap_set_snaplen(_handle, DEFAULT);

    }

    //promisous mode
    if (_options.promisc_mode) {
      pcap_set_promisc(_handle, 1);
    }

    //time between checking packet buffer
    if (_options.high_traffic) {
      pcap_set_timeout(_handle, 10);
    } else {
      pcap_set_timeout(_handle, 500);
    }

    if(pcap_findalldevs(&_device_list, errbuf) != 0) {
      std::cerr << "Error finding devices: " << errbuf << std::endl;
      return;
    }

  }

  void OnlinePacketCapture::start_capture() {

    pcap_loop(
    _handle,
    _packets_to_capture, 
    NULL, 
    NULL
    );

    return;

  }
}