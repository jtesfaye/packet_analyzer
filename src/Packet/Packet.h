
#ifndef PACKET_H
#define PACKET_H

#include "pcap/pcap.h"
#include <iostream>
#include<optional>
#include <cstdint>

namespace packet {

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

  namespace frame {

    struct EN10MB {

      u_int8_t dest_addr[6];
      u_int8_t src_addr[6];
      u_int16_t ether_type;
    
    };

    struct EN10MB_802_1_Q {

      u_int8_t dest_addr[6];
      u_int8_t src_addr[6];
      u_int16_t tpid;
      u_int16_t tci;
      u_int16_t ether_type;

    };

    struct _802_11 {

    };

  }

  namespace ip {

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

  }
  
  namespace transport {

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
  }

  namespace layers {

    union link_layer {

      const packet::frame::_802_11* ether_wireless;
      const packet::frame::EN10MB* ether_frame;
      const packet::frame::EN10MB_802_1_Q* vlan_ether_frame;

    };

    union ip_layer {

      packet::ip::ipv4_header* v4;
      packet::ip::ipv6_header v6;

    };

    union transport_layer {
      packet::transport::tcp_header tcp;
    };

  }


  class Packet {
  public:

    Packet() = delete;

    Packet
    (
      layers::link_layer&&
    );

  private:

    layers::link_layer l2;
    layers::ip_layer l3;
    layers::transport_layer l4;

  };

}

#endif