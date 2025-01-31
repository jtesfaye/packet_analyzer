
#ifndef PACKET_H
#define PACKET_H

#include "pcap/pcap.h"
#include "../parsing/LinkLayer.h"

#include <variant>
#include <functional>
#include <iostream>
#include<optional>
#include <cstdint>

namespace packet {

  
  namespace frame {

    constexpr uint16_t VLAN_PCP = 0xE000;
    constexpr uint16_t VLAN_DEI = 0x1000;
    constexpr uint16_t VLAN_VID = 0x0fff;

    enum _802_11_Masks : u_int16_t {

      PROTOCOL_VERS = 0xC000,
      TYPE = 0x3000,
      SUBTYPE = 0x0f00,
      ToDS = 0x0080,
      FromDS = 0040,
      MF = 0x0020,
      RETRY = 0x0010,
      PWRMGMT = 0x0008,
      MOREDATA = 0x0004,
      PROTECTED = 0x0002,
      ORDER = 0x0001,
      DATAFRAME = 0x2000,
      MANAGMENT = 0x0000,
      CONTROL = 0x1000
      
    };

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

      u_int16_t frame_control;
      u_int16_t duration_id;
      u_int8_t addr1[6];
      u_int8_t addr2[6];
      u_int8_t addr3[6];
      u_int16_t sqnc_ctrl;
      u_int8_t arrd4[6];

    };

    struct _802_2 {

      u_int8_t DSAP_addr;
      u_int8_t SSAP_addr;
      u_int8_t control;

    };

    struct snap_extension {

      u_int8_t oui[3]; //organizationally unique identifier, not 'yes' in french
      u_int16_t protocol_id;

    };

  }

  namespace ip {
    
    //NT means network type
    constexpr u_int16_t NT_IPV4 = 0x0800;
    constexpr u_int16_t NT_IPV6 = 0x08DD;
    constexpr u_int16_t NT_UNSUPPORTED = 0x0000;

    constexpr uint16_t IP_RF = 0x8000;
    constexpr uint16_t IP_DF = 0x4000;
    constexpr uint16_t IP_MF = 0x2000;
    constexpr uint16_t IP_OF = 0x1fff;


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

    constexpr u_int8_t TCP_OFFSET = 0xf0;
    constexpr u_int8_t TCP_CWR = 0x80;
    constexpr u_int8_t TCP_ECE = 0x40;
    constexpr u_int8_t TCP_URG = 0x20;
    constexpr u_int8_t TCP_ACK = 0x10;
    constexpr u_int8_t TCP_PSH = 0x08;
    constexpr u_int8_t TCP_RST = 0x04;
    constexpr u_int8_t TCP_SYN = 0x02;
    constexpr u_int8_t TCP_FIN = 0x01;


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

    struct upd_header {

    };
  }

  using link_layer = std::variant<
    std::monostate,
    const frame::EN10MB*,
    const frame::EN10MB_802_1_Q*,
    const frame::_802_11*
  >;

  using net_layer = std::variant<
    std::monostate,
    const ip::ipv4_header*,
    const ip::ipv6_header*
  >; 

  using transport_layer = std::variant<
    std::monostate,
    const transport::tcp_header*,
    const transport::upd_header*
  >; 

  class Packet {
  public:

    Packet() = delete;

    Packet
    (
      const int dlt,
      const u_int8_t flags,
      const u_int8_t*& raw_data
    );

    Packet(const Packet&) = delete;
    Packet operator=(const Packet&) = delete;
    Packet& operator= (Packet&&) = delete;

    ~Packet() {};

    void start_extract();

  private:

    struct packet_data {

      link_layer l2;

      net_layer l3;

      transport_layer l4;

      std::function<void(const link_layer&)> l2_display;
      std::function<void(const net_layer&)> l3_display;
      std::function<void(const transport_layer&)> l4_display;

    };

    class display_data {
    public:

      static void show_802_11(const link_layer& l2);
      
    };

    packet_data m_data; 

    parse::LinkParse getLayer2;

    int m_dlt;
    const u_int8_t m_flags;
    const u_int8_t* &m_raw_data;

  };

}

#endif