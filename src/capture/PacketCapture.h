
#ifndef PACKETCAPTURE_H
#define PACKETCAPTURE_H

#include <pcap/pcap.h>
#include <iostream>

namespace Capture {

  struct Options {

    bool full_capture; //if true will capture 65535 bytes of the packet.
    bool promisc_mode; //
    bool monitor_mode;
    bool immediate_mode;
    bool precision_mode;

  };

  class PacketCapture {
  protected:

    const char* _device;
    char errbuf[PCAP_ERRBUF_SIZE];

    pcap_t* _handle;
    pcap_if_t _device_list;

    int _packets_to_capture;
    Options _options;

    PacketCapture() = delete;
    PacketCapture(const char* device_name, int cnt, Options&& opt); 

    PacketCapture(PacketCapture&) = delete;
    PacketCapture operator=(PacketCapture&) = delete;

    ~PacketCapture() = default;

  public:

    pcap_t* handle() { return _handle; }
    pcap_if_t devices() { return _device_list; }

  };

}



#endif