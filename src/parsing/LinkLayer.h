
#ifndef LINKLAYER_H
#define LINKLAYER_H

#include "PacketParsing.h"
#include "Packet.h"
#include <functional>

namespace Parse {

  constexpr int DLT_TYPES = 10;

  class LinkParse {
  public:

    LinkParse
    (
      u_int8_t& raw_data, 
      std::function<link_layer(u_int8_t&)> func,
      int dlt
    );

  private:

    std::function<link_layer(u_int8_t&)> layer_func;

    u_int8_t& raw_data;

    link_layer operator()() {
      layer_func(raw_data);
    }



    class link_dispatch {
    public:

      std::function<link_layer(u_int8_t&)> table[DLT_TYPES];

      link_dispatch();

      link_dispatch(link_dispatch&) = delete;
      link_dispatch operator= (link_dispatch&) = delete;
      link_dispatch(link_dispatch&&) = delete;

      ~link_dispatch();

      std::function<link_layer(u_int8_t&)> operator[] (int key);

    };



    class link_parse_functions {
    public:

      link_parse_functions() = delete;

      static link_layer _EN10MB_parse(u_int8_t&);

    };

  };

}


#endif
