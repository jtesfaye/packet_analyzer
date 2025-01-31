
#ifndef LINKLAYER_H
#define LINKLAYER_H

#include "../Packet/Packet.h"
#include <arpa/inet.h>
#include <functional>
#include <variant>

using namespace packet;
using namespace packet::frame;

namespace parse {

  constexpr int DLT_TYPES = 10;

  class LinkParse {
  public:

    explicit LinkParse(int dlt);

    ~LinkParse() {};

    link_layer operator()(const u_int8_t* raw_data) {
      return layer_func(raw_data, *this);
    }

    u_int16_t layer3_type();
    u_int8_t header_length();

  private:

    std::function<link_layer(const u_int8_t*, LinkParse&)> layer_func;

    void set_layer3_type(u_int16_t);
    void set_length(u_int8_t);

    u_int16_t m_layer3_type; //used to tell the the NetworkParse object what protocol it will use 
    u_int8_t m_header_length; //header length in bytes
    bool data_frame;

    class link_dispatch {
    public:

      std::function<link_layer(const u_int8_t*, LinkParse&)> table[DLT_TYPES];

      link_dispatch(LinkParse&);

      link_dispatch(link_dispatch&) = delete;
      link_dispatch operator= (link_dispatch&) = delete;
      link_dispatch(link_dispatch&&) = delete;

      ~link_dispatch() {};

      std::function<link_layer(const u_int8_t*, LinkParse&)> operator[] (int key);

    };

    class link_parse_functions {
    public:

      link_parse_functions() = delete;

      static link_layer _EN10MB_parse(const u_int8_t*, LinkParse&);
      static link_layer _802_11_parse(const u_int8_t*, LinkParse&);

    };

  };

}


#endif
