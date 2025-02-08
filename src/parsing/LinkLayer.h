
#ifndef LINKLAYER_H
#define LINKLAYER_H

#include "../Packet/ProtocolTypes.h"
#include <pcap/pcap.h>

#include <functional>
#include <utility>
#include <array>

using namespace packet;
using namespace packet::frame;

namespace parse {

  constexpr int DLT_TYPES = 10;

  class LinkParse {

    using function = std::function<link_layer(const u_int8_t*, LinkParse&)>;
    using key_pair = std::pair<int, function>;

  public:

    explicit LinkParse (int dlt);

    ~LinkParse() {};

    link_layer operator()(const u_int8_t* raw_data) {
      return layer_func(raw_data, *this);
    }

    u_int16_t layer3_type();
    u_int8_t header_length();

    
    void set_capture_data(const pcap_pkthdr* &header);

  private:

    std::function<link_layer(const u_int8_t*, LinkParse&)> layer_func;

    void set_layer3_type(u_int16_t);
    
    void set_length(u_int8_t);

    u_int16_t m_layer3_type; //used to tell the the NetworkParse object what protocol it will use 
    u_int8_t m_header_length; //header length in bytes
    const pcap_pkthdr* m_capture_data;

    class link_parse_functions {
    public:
      link_parse_functions() = delete;

      static constexpr int total {2};
      
      static link_layer _EN10MB_parse(const u_int8_t*, LinkParse&);
      static link_layer _802_11_parse(const u_int8_t*, LinkParse&);

    };

    const std::array<key_pair, link_parse_functions::total> all_parse_functions 
    {
        key_pair {DLT_EN10MB, function(link_parse_functions::_EN10MB_parse)},
        key_pair {DLT_IEEE802_11, function(link_parse_functions::_802_11_parse)}
    };

  };

}


#endif
