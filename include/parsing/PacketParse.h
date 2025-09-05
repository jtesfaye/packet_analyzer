
#ifndef PACKETPARSE_H
#define PACKETPARSE_H

#include "pcap/pcap.h"
#include <packet/LayerWrappers.h>
#include <parsing/ParseDispatcher.h>
#include <functional>

namespace parse {

  constexpr u_int8_t DO_LAYER3 = 0x40;
  constexpr u_int8_t DO_LAYER4 = 0x20;

  using namespace packet;

  class PacketParse {
  public:

    PacketParse(int dlt, u_int8_t flags);

    PacketParse(const PacketParse&) = delete;
    PacketParse operator=(const PacketParse&) = delete;
    PacketParse& operator= (PacketParse&&) = delete;

    ~PacketParse() = default;

    std::pair<row_entry,packet_ref> start_extract(const std::vector<std::byte> &raw_data);

  private:

    struct LayerJob {

      std::function<void(packet_ref&, const std::vector<std::byte>&, parse_context&, layer_offsets&)> func;

    };

    std::vector<LayerJob> create_jobs(u_int8_t flags);

    int m_dlt; //data link type

    u_int8_t m_flags;

    timeval m_inital_time; //time at which first packet was captured

    ParseDispatcher<link_layer_ref, true> link_parser;

    ParseDispatcher<net_layer_ref, false> net_parser;

    ParseDispatcher<transport_layer_ref, false> transport_parser;

    void set_inital_time(timeval& time);

    double set_relative_time(const timeval& time) const;

    bool init_time_has_value() const;


  };

}

#endif