
#ifndef PACKET_H
#define PACKET_H

#include "pcap/pcap.h"
#include "ProtocolTypes.h"
#include "../parsing/LinkLayer.h"
#include <memory>
#include <functional>
#include <utility>
#include <array>

namespace parse {
  class LinkParse;
}

namespace packet {

  struct packet_model {

    link_layer l2;

    net_layer l3;

    transport_layer l4;

    std::function<void(const link_layer&)> l2_display;
    std::function<void(const net_layer&)> l3_display;
    std::function<void(const transport_layer&)> l4_display;

  };

  class Packet {
  public:

    Packet();

    Packet(const Packet&) = delete;
    Packet operator=(const Packet&) = delete;
    Packet& operator= (Packet&&) = delete;

    ~Packet() {};

    void init(int dlt, u_int8_t flags);

    packet_model start_extract();

    void set_meta_data(const pcap_pkthdr* &meta);

    void set_raw_data(const u_int8_t* &raw_data);


  private:
  
    class display_data {
    public:

      using display_type = void(const link_layer&);
      using function = std::function<display_type>;
      using key_pair = std::pair<int, function>;

      static constexpr int size {2};

      static void show_802_11(const link_layer& l2);
      static void show_EN10MB(const link_layer& l2);
      
    }; 

    std::unique_ptr<parse::LinkParse> layer2_parse;

    void set_layer2_display(packet_model& pkt);

    int m_dlt;
    u_int8_t m_flags;

    const u_int8_t* m_raw_data;
    const pcap_pkthdr* m_meta_data;

    const std::array<display_data::key_pair, display_data::size> m_all_l2_display
    {
      display_data::key_pair {DLT_IEEE802_11, display_data::function(display_data::show_802_11)},
      display_data::key_pair {DLT_EN10MB, display_data::function(display_data::show_EN10MB)}
    };

  };

}

#endif