
#ifndef PACKETCAPTURE_H
#define PACKETCAPTURE_H

#include <pcap/pcap.h>
#include <iostream>

namespace Capture {

  const int FULL = 65535;
  const int DEFAULT = 256;
  const int BASIC = 128;

  struct Options {

    //between full or basic, only one of them can be true
    bool full_capture;                                                              //if true will capture 65535 bytes of the packet.
    bool basic_capture;                                                             // if true will capture 128 bytes
    
    bool promisc_mode = false;                                                    
    bool monitor_mode;
    bool immediate_mode = true;
    bool precision_mode;
    bool high_traffic;

  };

  class PacketCapture {
  protected:

    const char* _device;
    char errbuf[PCAP_ERRBUF_SIZE];

    pcap_t* _handle;
    pcap_if_t* _device_list;

    int _packets_to_capture;
    Options _options;

    virtual void start_capture() = 0;
    virtual void stop_capture() = 0;

  public:

    PacketCapture() = delete;
    PacketCapture(const char* device_name, int cnt, const Options&& opt);

    PacketCapture(PacketCapture&) = delete;
    PacketCapture operator=(PacketCapture&) = delete;

    virtual ~PacketCapture() = default; 

    pcap_t* handle() { return _handle; }
    pcap_if_t* devices() { return _device_list; }


  };

}



#endif