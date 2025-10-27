
#ifndef INITIALPARSER_H
#define INITIALPARSER_H

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

class InitialParser {
public:

  InitialParser(int dlt, u_int8_t flags);

  InitialParser(const InitialParser&) = delete;
  InitialParser operator=(const InitialParser&) = delete;
  InitialParser& operator= (InitialParser&&) = delete;

  ~InitialParser() = default;

  packet_ref start_extract(
    const std::vector<std::byte> &raw_data,
    size_t index);

  std::vector<ProtocolDetails> detail_parse(
    const std::vector<std::byte> &raw_data,
    const layer_offsets &data);

private:

  struct LayerJob {

    std::function<bool(
      packet_ref&,
      const std::vector<std::byte>&,
      parse_context&,
      layer_offsets&)> func;

    std::function<ProtocolDetails(
      const std::vector<std::byte>&,
      parse_context&,
      const layer_offsets&)> detail_func;
    };

  std::vector<LayerJob> create_first_parse_jobs();

  std::vector<LayerJob> create_detail_parse_jobs();

  void set_initial_time(const parse_time& time);

  double set_relative_time(const parse_time& time);

  int m_dlt; //data link type

  u_int8_t m_flags;

  parse_time m_inital_time; //time at which first packet was captured

  std::once_flag time_init_flag;

  ParseDispatcher<std::unique_ptr<ProtocolDataUnit>> first_parse_dispatcher;

  ParseDispatcher<ProtocolDetails> detail_parse_dispatcher;

};



#endif