
#ifndef PROTOCOLTYPES_H
#define PROTOCOLTYPES_H

#include <cstdint>
#include <variant>

namespace packet {


  namespace frame {

    constexpr uint16_t VLAN_PCP = 0xE000;
    constexpr uint16_t VLAN_DEI = 0x1000;
    constexpr uint16_t VLAN_VID = 0x0fff;

    namespace _802_11_mask {

      constexpr u_int16_t PROTOCOL_VERS = 0xC000;
      constexpr u_int16_t TYPE = 0x3000;
      constexpr u_int16_t SUBTYPE = 0x0f00;
      constexpr u_int16_t ToDS = 0x0080;
      constexpr u_int16_t FromDS = 0040;
      constexpr u_int16_t MF = 0x0020;
      constexpr u_int16_t RETRY = 0x0010;
      constexpr u_int16_t PWRMGMT = 0x0008;
      constexpr u_int16_t MOREDATA = 0x0004;
      constexpr u_int16_t PROTECTED = 0x0002;
      constexpr u_int16_t ORDER = 0x0001;
      constexpr u_int16_t DATAFRAME = 0x2000;
      constexpr u_int16_t MANAGMENT = 0x0000;
      constexpr u_int16_t CONTROL = 0x1000;

    }

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





}


#endif