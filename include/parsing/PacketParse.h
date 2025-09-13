
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

  using namespace packet;

  class PacketParse {
  public:

    PacketParse(int dlt, u_int8_t flags);

    PacketParse(const PacketParse&) = delete;
    PacketParse operator=(const PacketParse&) = delete;
    PacketParse& operator= (PacketParse&&) = delete;

    ~PacketParse() = default;

    std::pair<row_entry,packet_ref> start_extract(const std::vector<std::byte> &raw_data, size_t index);

  private:

    struct LayerJob {

      std::function<void(packet_ref&, const std::vector<std::byte>&, parse_context&, layer_offsets&)> func;

    };

    std::vector<LayerJob> create_jobs(u_int8_t flags);

    int m_dlt; //data link type

    u_int8_t m_flags;

    timeval m_inital_time; //time at which first packet was captured

    std::once_flag time_init_flag;

    ParseDispatcher<std::unique_ptr<LinkPDU>, true> link_parser;

    ParseDispatcher<std::unique_ptr<NetworkPDU>, false> net_parser;

    ParseDispatcher<std::unique_ptr<TransportPDU>, false> transport_parser;

    void set_inital_time(const timeval& time);

    double set_relative_time(const timeval& time);


  };

}

#endif