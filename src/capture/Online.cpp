
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

    //rf mode
    if (_options.monitor_mode) {

      int rf = pcap_can_set_rfmon(_handle);
      if (rf) {
        pcap_set_rfmon(_handle, 1);
      } else {
        //send message to user than rf mode cant be used on this device
      }
    }

    //decided if packet gets immediatly delivered instead of going to buffer
    if (_options.immediate_mode) {

      pcap_set_immediate_mode(_handle, 1);

    }

    //precision of timestamps
    if (_options.precision_mode) {

      pcap_set_tstamp_type(_handle, PCAP_TSTAMP_HOST_HIPREC);

    } else {

      pcap_set_tstamp_type(_handle, PCAP_TSTAMP_HOST);

    }

    //time between checking packet buffer
    if (_options.high_traffic) {

      pcap_set_timeout(_handle, 10);

    } else {

      pcap_set_timeout(_handle, 500);

    }

    if(pcap_findalldevs(&_device_list, errbuf) != 0) {

      std::cerr << "Error finding devices: " << errbuf << std::endl;

    }

  }


  void OnlinePacketCapture::process_packet
  (
    u_char* user_data,
    const struct pcap_pkthdr* header,
    const u_char* packet
  ) {


  }
  
  void OnlinePacketCapture::start_capture() {

    pcap_loop(
    _handle,
    _packets_to_capture, 
    process_packet, 
    NULL
    );

    return;

  }
}