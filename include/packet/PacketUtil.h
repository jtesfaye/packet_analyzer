//
// Created by jeremiah tesfaye on 7/27/25.
//

#ifndef PACKETUTIL_H
#define PACKETUTIL_H

#include <array>
#include <pcap/pcap.h>
#include <sstream>
#include <iomanip>

struct LinkPDU;
struct NetworkPDU;
struct TransportPDU;

namespace packet {

  struct row_entry {

    size_t index;
    double time;
    std::string src;
    std::string dest;
    std::string protocol;
    size_t length;
    std::string info;

    static row_entry make_row_entry(size_t index,
      double time,
      std::string src,
      std::string dest,
      std::string protocol,
      size_t length,
      std::string info) {

      return {
        index,
        time,
        std::move(src),
        std::move(dest),
        std::move(protocol),
        length,
        std::move(info)
      };

    }

    [[nodiscard]] std::array<std::string, 6> to_array() const {

      std::ostringstream oss;
      oss << std::fixed << std::setprecision(10) << time;

      return {oss.str(), src, dest, protocol, std::to_string(length), info};
    }

  };

  struct parse_context {

    pcap_pkthdr header;     //header of packet, which is present in all packets captured by pcap
    u_int16_t next_type;    //Holds the value assigned to a protocol by the IEEE or IANA
    size_t offset;          //start of the protocol
    size_t curr_length;     //Represents length of protocol that was just parsed (set by parser)
    size_t prev_length;     //Length of previous protocol, used to find the start of the next protocol

  };

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

    std::unique_ptr<LinkPDU> layer2;

    std::unique_ptr<NetworkPDU> layer3;

    std::unique_ptr<TransportPDU> layer4;

  };

  struct pcaprec_hdr_t {

    uint32_t ts_sec;   // timestamp seconds
    uint32_t ts_usec;  // timestamp microseconds
    uint32_t incl_len; // number of bytes of packet saved in file
    uint32_t orig_len; // actual length of packet

  } __attribute__((packed));
}



#endif //PACKETUTIL_H
