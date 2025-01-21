
#include "Online.h"


namespace capture {

  Online::Online
  (
    const char* device_name,
    int count, 
    u_int8_t set
  ) 
  : PacketCapture(device_name, count, set) {

    _handle = pcap_create(_device, errbuf);

    if (_handle == nullptr) {
      std::cerr << "Error in creating handle: " << errbuf << std::endl;
      return; //probably not the best way to handle an error but ill change it later
    }

    //decide how much of the packet will be captures
    if (_settings & FULL_CAP == FULL_CAP) {

      pcap_set_snaplen(_handle, FULL);
      
    } else if (_settings & BASIC_CAP == BASIC_CAP) {

      pcap_set_snaplen(_handle, BASIC);

    } else {

      pcap_set_snaplen(_handle, DEFAULT);

    }

    //promisous mode
    if (_settings & PROMISC == PROMISC) {
      pcap_set_promisc(_handle, 1);
    }

    //rf mode
    if (_settings & MONITOR == MONITOR) {

      int rf = pcap_can_set_rfmon(_handle);
      if (rf) {
        pcap_set_rfmon(_handle, 1);
      } else {
        //send message to user than rf mode cant be used on this device
      }
    }

    //decided if packet gets immediatly delivered instead of going to buffer
    if (_settings & IMMEDIATE == IMMEDIATE) {

      pcap_set_immediate_mode(_handle, 1);

    }

    //precision of timestamps
    if (_settings & PRECISION == PRECISION) {

      pcap_set_tstamp_type(_handle, PCAP_TSTAMP_HOST_HIPREC);

    } else {

      pcap_set_tstamp_type(_handle, PCAP_TSTAMP_HOST);

    }

    //time between checking packet buffer
    if (_settings & HIGH_TRAFF == HIGH_TRAFF) {

      pcap_set_timeout(_handle, 10);

    } else {

      pcap_set_timeout(_handle, 500);

    }

    if(pcap_findalldevs(&_device_list, errbuf) != 0) {

      std::cerr << "Error finding devices: " << errbuf << std::endl;

    }

    std::cout << "Capturing began successfully\n";

  }

  Online::~Online() {};


  void 
  Online::process_packet
  (
    u_char* user_data,
    const struct pcap_pkthdr* header,
    const u_char* packet
  ) {


  }
  
  void 
  Online::start_capture() {

    pcap_loop(_handle, _packets_to_capture, process_packet, NULL);

    return;

  }

  void Online::stop_capture() {

  }
}