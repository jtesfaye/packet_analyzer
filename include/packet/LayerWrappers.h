//
// Created by jeremiah tesfaye on 7/27/25.
//

#ifndef LAYERWRAPPERS_H
#define LAYERWRAPPERS_H

#include <packet/ProtocolTypes.h>
#include <pcap/pcap.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <variant>

namespace packet {

  struct row_entry {

    size_t index;
    double time;
    std::string src;
    std::string dest;
    std::string protocol;
    std::string length;
    std::string info;

    [[nodiscard]] std::array<std::string, 6> to_array() const {

      std::ostringstream oss;
      oss << std::fixed << std::setprecision(10) << time;

      return {oss.str(), src, dest, protocol, length, info};
    }

  };

  struct HeaderBase {



  };

  struct parse_context {

    pcap_pkthdr header;
    u_int16_t next_type;
    size_t offset;
    u_int16_t length;

  };


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



  struct LayerRegion {
    size_t offset;
    size_t length;
  };

  struct layer_offsets {

    LayerRegion l2;
    LayerRegion l3;
    LayerRegion l4;

  };

  struct packet_ref {

    layer_offsets data;

    link_layer_ref layer2;

    net_layer_ref layer3;

    transport_layer_ref layer4;

  };

  struct pcaprec_hdr_t {

    uint32_t ts_sec;   // timestamp seconds
    uint32_t ts_usec;  // timestamp microseconds
    uint32_t incl_len; // number of bytes of packet saved in file
    uint32_t orig_len; // actual length of packet

  } __attribute__((packed));
}



#endif //LAYERWRAPPERS_H
