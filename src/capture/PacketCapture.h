
#ifndef PACKETCAPTURE_H
#define PACKETCAPTURE_H

#include <pcap/pcap.h>
#include "../Packet/Packet.h"
#include <memory>
#include <string>

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
    std::string _file_path;
    char errbuf[PCAP_ERRBUF_SIZE];

    static void free_devices(pcap_if_t* devlist);
    static void close_handle(pcap_t* handle);

    packet::Packet _packet_data;

    std::unique_ptr<pcap_t, decltype(&close_handle)> _handle;
    std::unique_ptr<pcap_if_t, decltype(&free_devices)> _device_list;

    int _packets_to_capture;

    u_int8_t _settings;

    void 
    process_packet
    (const struct pcap_pkthdr *head, const u_char *bytes);

    virtual void 
    get_devices() {};

    virtual void 
    get_link_types() {};

    inline pcap_t* 
    handle() { 
      return _handle.get(); 
    }

    pcap_if_t* 
    devices() { 
      return _device_list.get(); 
    }

    PacketCapture() = delete;
    PacketCapture(std::string device_name, int cnt, const u_int8_t settings, u_int8_t flags);
    PacketCapture(std::string path_name, int cnt, u_int8_t flags);

  public:

    void
    start_capture();

    void 
    stop_capture();

    PacketCapture(PacketCapture&) = delete;
    PacketCapture operator=(PacketCapture&) = delete;

    virtual ~PacketCapture();

  };

}



#endif