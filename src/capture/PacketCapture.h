
#ifndef PACKETCAPTURE_H
#define PACKETCAPTURE_H

#include <pcap/pcap.h>
#include <iostream>

namespace capture {

  constexpr int FULL = 65535;
  constexpr int DEFAULT = 256;
  constexpr int BASIC = 128;

  constexpr u_int8_t FULL_CAP = 0x80;
  constexpr u_int8_t BASIC_CAP = 0x40;
  constexpr u_int8_t PROMISC = 0x20;
  constexpr u_int8_t IMMEDIATE = 0x10;
  constexpr u_int8_t MONITOR = 0x08;
  constexpr u_int8_t PRECISION = 0x04;
  constexpr u_int8_t HIGH_TRAFF = 0x02;

  class PacketCapture {
  protected:

    const char* _device;
    char errbuf[PCAP_ERRBUF_SIZE];

    pcap_t* _handle;
    pcap_if_t* _device_list;

    int _packets_to_capture;

    u_int8_t _settings;

    virtual void start_capture() = 0;
    virtual void stop_capture() = 0;

  public:

    PacketCapture() = delete;
    PacketCapture(const char* device_name, int cnt, const u_int8_t settings);

    PacketCapture(PacketCapture&) = delete;
    PacketCapture operator=(PacketCapture&) = delete;

    virtual ~PacketCapture();

    pcap_t* handle() { return _handle; }
    pcap_if_t* devices() { return _device_list; }


  };

}



#endif