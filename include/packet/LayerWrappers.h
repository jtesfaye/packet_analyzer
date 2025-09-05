//
// Created by jeremiah tesfaye on 7/27/25.
//

#ifndef LAYERWRAPPERS_H
#define LAYERWRAPPERS_H

#include "ProtocolTypes.h"
#include <pcap/pcap.h>

namespace packet {

  struct parse_context {

    const pcap_pkthdr* header;
    u_int16_t layer3_type;
    size_t offset;
    const uint8_t* data;
    u_int16_t length;

  };

  using link_layer_ref = std::variant<
    std::monostate,
    frame::EN10MB*,
    frame::EN10MB_802_1_Q*,
    frame::_802_11*
  >;

  using link_layer = std::variant<
    std::monostate,
    frame::EN10MB,
    frame::EN10MB_802_1_Q,
    frame::_802_11
  >;

  using net_layer_ref = std::variant<
    std::monostate,
    ip::ipv4_header*,
    ip::ipv6_header*
  >;

  using net_layer = std::variant<
    std::monostate,
    ip::ipv4_header,
    ip::ipv6_header
  >;

  using transport_layer_ref = std::variant<
    std::monostate,
    transport::tcp_header*,
    transport::upd_header*
  >;

  struct packet_ref {

    link_layer_ref l2;

    net_layer_ref l3;

    transport_layer_ref l4;

  };
}



#endif //LAYERWRAPPERS_H
