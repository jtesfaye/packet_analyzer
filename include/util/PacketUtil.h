//
// Created by jeremiah tesfaye on 7/27/25.
//

#ifndef PACKETUTIL_H
#define PACKETUTIL_H

#include <pcap/pcap.h>
#include <sstream>
#include <vector>
#include <layerx/ProtocolDataUnit.h>
#include <span>
#include <stream/StreamStatistics.h>
#include <boost/json/object.hpp>
#include <queue>
struct LinkPDU;
struct NetworkPDU;
struct TransportPDU;

namespace packet {

  bool valid_length(std::span<std::byte> data, size_t offset, size_t needed);
  std::string format_mac(const u_int8_t* addr);
  std::string format_ipv4_src_dst(const u_int32_t& addr);
  std::string format_ipv6_src_dest(const u_int8_t* addr);
  std::string protocol_to_string(u_int16_t protocol);

  struct RawPacket {
    size_t index;
    std::byte packet[1500];
  };

  class raw_packet_queue
  {
  public:
    void push(const RawPacket& pkt)
    {
      std::unique_lock ul(lock);
      _queue.push(pkt);
    }
    RawPacket pop()
    {
      std::unique_lock ul(lock);
      const auto data = _queue.front();
      _queue.pop();
      return data;
    }
    bool empty()
    {
      std::unique_lock ul(lock);
      return _queue.empty();
    }
  private:
    std::mutex lock;
    std::queue<RawPacket> _queue;
  };

  struct timestamp {
    uint32_t ts_sec;
    uint32_t ts_usec;
  };

  struct pcaprec_hdr_t {

    uint32_t ts_sec;   // timestamp seconds
    uint32_t ts_usec;  // timestamp microseconds
    uint32_t incl_len; // number of bytes of packet saved in file
    uint32_t orig_len; // actual length of packet

  } __attribute__((packed));

  struct parse_context {
    pcaprec_hdr_t header;    //header of packet, which is present in all packets captured by pcap
    u_int16_t next_type;    //Holds the value assigned to a protocol by the IEEE or IANA
    size_t offset;          //start of the protocol
    size_t curr_length;     //Represents length of protocol that was just parsed (set by parser)
    size_t prev_length;     //Length of previous protocol, used to find the start of the next protocol
    bool is_fragmented;
  };

  struct LayerRegion {
    size_t offset;
    size_t length;
    int protocol_type;
  };

  struct layer_offsets {

    LayerRegion l2;
    LayerRegion l3;
    LayerRegion l4;

  };

  class packet_data {
  public:

    size_t index{};
    timestamp time{};
    size_t wire_length{};
    size_t payload_length{};
    layer_offsets data{};
    size_t stream_index;
    std::shared_ptr<ProtocolDataUnit> layer2;
    std::shared_ptr<ProtocolDataUnit> layer3;
    std::shared_ptr<ProtocolDataUnit> layer4;

    packet_data();
    packet_data(const packet_data&) = default;
    ~packet_data();
    packet_data(packet_data&&) noexcept;
    packet_data& operator=(packet_data&&) noexcept;
    packet_data(const packet_data&&) = default;
    packet_data& operator=(const packet_data&) = default;
  };

  struct ProtocolDetails {
    std::string_view name;
    std::vector<std::string> fields;
  };

  boost::json::object to_json(const packet_data& ref);
  boost::json::object to_json(const ProtocolDetails& ref);
  boost::json::object to_json(const StreamStatistics& stats);


}



#endif //PACKETUTIL_H
