
#ifndef PACKET_H
#define PACKET_H

#include "pcap/pcap.h"
#include <cstdint>
#include <optional>

namespace Packet {

  constexpr uint16_t IP_RF = 0x8000;
  constexpr uint16_t IP_DF = 0x4000;
  constexpr uint16_t IP_MF = 0x2000;
  constexpr uint16_t IP_OF = 0x1fff;

  constexpr uint16_t VLAN_PCP = 0xE000;
  constexpr uint16_t VLAN_DEI = 0x1000;
  constexpr uint16_t VLAN_VID = 0x0fff;

  constexpr u_int8_t TCP_OFFSET = 0xf0;
  constexpr u_int8_t TCP_CWR = 0x80;
  constexpr u_int8_t TCP_ECE = 0x40;
  constexpr u_int8_t TCP_URG = 0x20;
  constexpr u_int8_t TCP_ACK = 0x10;
  constexpr u_int8_t TCP_PSH = 0x08;
  constexpr u_int8_t TCP_RST = 0x04;
  constexpr u_int8_t TCP_SYN = 0x02;
  constexpr u_int8_t TCP_FIN = 0x01;

  struct _802_3 {
    u_int8_t dest_addr[6];
    u_int8_t src_addr[6];
    std::optional<_802_1_Q> vlan_tag;
    u_int16_t ether_type;
  };

  struct _802_1_Q {

    u_int16_t tpid;
    u_int16_t tci;

  };

  struct _802_11 {

  };

  struct ipv4_header {

  #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    u_int8_t version : 4;
    u_int8_t ih_length : 4;

  #endif
  #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    u_int8_t ih_length : 4;
    u_int8_t version : 4;

  #endif
    u_int8_t dscp : 6;
    u_int8_t ecn : 2;
    u_int16_t length;
    u_int16_t id;
    u_int8_t ttl;
    u_int8_t protocol;
    u_int16_t chksum;
    u_int32_t src_addr;
    u_int32_t dest_adr;

  };

  struct ipv6_header {

  };

  struct tcp_header {

    u_int16_t src;
    u_int16_t dest;
    u_int32_t sequence;
    u_int32_t ack;
    u_int8_t offset;
    u_int8_t flags;
    u_int16_t window;
    u_int16_t chksum;
    u_int16_t urgent;

  };

    union link_layer {

      _802_11 ether_wireless;
      _802_3 ether_frame;

    };

    union ip_layer {

      ipv4_header v4;
      ipv6_header v6;

    };

    union transport_layer {
      tcp_header tcp;
    };

  class Packet {
  public:

    Packet() = delete;

    Packet
    (
      const struct pcap_pkthdr& header,
      link_layer&&,
      ip_layer&&,
      transport_layer&&
    );

  private:

    std::optional<link_layer> l2;
    ip_layer l3;
    transport_layer l4;

  };

}

#endif