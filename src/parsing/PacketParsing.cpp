
#include "PacketParsing.h"

namespace Parse {

PacketParse::PacketParse
(
  const struct pcap_pkthdr& hdr, 
  const u_int8_t& packet
) 
: _packet_head(hdr),
  _packet(packet) {}

}
