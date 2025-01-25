
#ifndef PACKETCAPTURE_H
#define PACKETCAPTURE_H

#include <pcap/pcap.h>
#include "../parsing/LinkLayer.h"
#include "../Packet/Packet.h"
#include <memory>
#include <optional>
#include <string>
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

    std::string _device;
    char errbuf[PCAP_ERRBUF_SIZE];

    struct user_data {
      Parse::LinkParse& link;
    };

    std::unique_ptr<pcap_t, decltype(&pcap_close)> _handle;
    std::unique_ptr<pcap_if_t, decltype(&pcap_freealldevs)> _device_list;

    int _packets_to_capture;

    u_int8_t _settings;

    virtual void start_capture() = 0;
    virtual void stop_capture() = 0;
    virtual void get_devices() = 0;
    virtual void get_link_types() = 0;

    pcap_t* handle() { return _handle.get(); }
    pcap_if_t* devices() { return _device_list.get(); }

  public:

    PacketCapture() = delete;
    PacketCapture(std::string device_name, int cnt, const u_int8_t settings);

    PacketCapture(PacketCapture&) = delete;
    PacketCapture operator=(PacketCapture&) = delete;

    virtual ~PacketCapture();



  };

}



#endif