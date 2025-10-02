
#ifndef PACKETPARSE_H
#define PACKETPARSE_H

#include <packet/PacketUtil.h>
#include <parsing/ParseDispatcher.h>
#include <layerx/ProtocolTypes.h>
#include <functional>
#include <mutex>

namespace parse {
  constexpr u_int8_t DO_LAYER3 = 0x40;
  constexpr u_int8_t DO_LAYER4 = 0x20;
}

using namespace packet;

class PacketParse {
public:

  PacketParse(int dlt, u_int8_t flags);

  PacketParse(const PacketParse&) = delete;
  PacketParse operator=(const PacketParse&) = delete;
  PacketParse& operator= (PacketParse&&) = delete;

  ~PacketParse() = default;

  packet_ref start_extract(
    const std::vector<std::byte> &raw_data,
    size_t index);

private:

  struct LayerJob {

    std::function<bool(
      packet_ref&,
      const std::vector<std::byte>&,
      parse_context&,
      layer_offsets&)> func;

  };

  std::vector<LayerJob> create_jobs();

  void set_initial_time(const parse_time& time);

  double set_relative_time(const parse_time& time);

  int m_dlt; //data link type

  u_int8_t m_flags;

  parse_time m_inital_time; //time at which first packet was captured

  std::once_flag time_init_flag;

  ParseDispatcher<std::unique_ptr<ProtocolDataUnit>, true> link_parser;

  ParseDispatcher<std::unique_ptr<ProtocolDataUnit>, false> net_parser;

  ParseDispatcher<std::unique_ptr<ProtocolDataUnit>, false> transport_parser;

};



#endif