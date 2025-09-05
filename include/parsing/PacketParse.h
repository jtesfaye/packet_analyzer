
#ifndef PACKETPARSE_H
#define PACKETPARSE_H

#include "pcap/pcap.h"
#include "tests/include/packet/LayerWrappers.h"
#include "tests/include/parsing/LinkParse.h"
#include <functional>

namespace parse {

  class PacketParse {
  public:

    PacketParse(int dlt, u_int8_t flags);

    PacketParse(const PacketParse&) = delete;
    PacketParse operator=(const PacketParse&) = delete;
    PacketParse& operator= (PacketParse&&) = delete;

    ~PacketParse() = default;

    packet_ref start_extract(const pcap_pkthdr* &meta, const u_int8_t* &raw_data);

  private:

    int m_dlt;

    u_int8_t m_flags;

    std::unique_ptr<LinkParse> layer2_parse;

  };

}

#endif