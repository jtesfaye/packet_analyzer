
#ifndef INITIALPARSER_H
#define INITIALPARSER_H

#include <packet/PacketUtil.h>
#include <parsing/ParseDispatcher.h>
#include <functional>
#include <mutex>
#include <span>

namespace parse {
  constexpr u_int8_t DO_LAYER3 = 0x40;
  constexpr u_int8_t DO_LAYER4 = 0x20;
}

using namespace packet;

class InitialParser {
public:

  InitialParser(int layer2_type, u_int8_t flags);

  InitialParser(const InitialParser&) = delete;
  InitialParser operator=(const InitialParser&) = delete;
  InitialParser& operator= (InitialParser&&) = delete;

  ~InitialParser() = default;

  packet_ref start_extract(
    std::span<std::byte> raw_data,
    size_t index);

private:

  struct LayerJob {

    std::function<bool(
      packet_ref&,
      std::span<std::byte>,
      parse_context&,
      layer_offsets&)> func;

    };

  std::vector<LayerJob> create_first_parse_jobs();

  void set_initial_time(const timestamp& time);

  int m_dlt; //data link type

  u_int8_t m_flags;

  timestamp m_inital_time; //time at which first packet was captured

  std::once_flag time_init_flag;

  ParseDispatcher<std::unique_ptr<ProtocolDataUnit>> first_parse_dispatcher;

};



#endif