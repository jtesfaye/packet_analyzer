//
// Created by jeremiah tesfaye on 12/20/25.
//

#ifndef IPACKETQUERYAPI_H
#define IPACKETQUERYAPI_H
#include <util/PacketUtil.h>
#include <session/Stream.h>
using namespace packet;
struct IPacketQueryAPI {
  virtual std::span<packet_ref> get_packets(size_t first, size_t last);
  virtual ProtocolDetails get_packet_details(size_t packet_id);
  virtual StreamStats get_stream_stats(size_t stream_index);
};

#endif //IPACKETQUERYAPI_H
