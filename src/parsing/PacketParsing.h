
#ifndef PACKETPARSING_H
#define PACKETPARSING_H

#include "pcap/pcap.h"
#include "Packet.h"
#include <cstdint>

using Packet::link_layer;
using Packet::ip_layer;
using Packet::transport_layer;

namespace Parse {

  struct raw_packet {

    const struct pcap_pkthdr header;
    const u_int8_t* raw_data;

  };

  class PacketParse {
  public:

    PacketParse(raw_packet&&, int dlt);

    ~PacketParse();

    PacketParse(PacketParse&) = delete;
    PacketParse operator= (PacketParse&) = delete;

    PacketParse(PacketParse&&) = delete;

    link_layer parse_link();
    ip_layer parse_ip();
    transport_layer parse_transport();


  private:

    const struct pcap_pkthdr _packet_head;
    const u_int8_t& _packet;
    const int dlt;




  };

};
#endif
