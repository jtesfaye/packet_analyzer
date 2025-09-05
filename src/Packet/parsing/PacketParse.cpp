
#include "tests/include/parsing/PacketParse.h"



namespace parse {

  PacketParse::PacketParse(int dlt, u_int8_t flags)
  : m_dlt{dlt}
  , m_flags{flags}
  , layer2_parse(std::make_unique<LinkParse>(dlt))
  {}

  packet_ref
  PacketParse::start_extract(const pcap_pkthdr* &meta, const u_int8_t* &raw_data) {

    packet_ref pkt;
    parse_context context{};

    context.header = meta;

    pkt.l2 = (*layer2_parse)(raw_data, context);

    if (m_flags & 0x40) {
      //call network parse using network type
    }

    if (m_flags & 0x20) {

    }

    return pkt;

  }


}