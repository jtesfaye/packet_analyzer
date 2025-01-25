
#ifndef LINKLAYER_H
#define LINKLAYER_H

#include "../Packet/Packet.h"
#include <arpa/inet.h>
#include <functional>

using packet::layers::link_layer;
using namespace packet::frame;


namespace Parse {

  constexpr int DLT_TYPES = 10;

  class LinkParse {
  public:

    explicit LinkParse(int dlt);

    ~LinkParse() {};

    link_layer operator()(const u_int8_t* raw_data) {
      return layer_func(raw_data);
    }

  private:

    std::function<link_layer(const u_int8_t*)> layer_func;


    class link_dispatch {
    public:

      std::function<link_layer(const u_int8_t*)> table[DLT_TYPES];

      link_dispatch();

      link_dispatch(link_dispatch&) = delete;
      link_dispatch operator= (link_dispatch&) = delete;
      link_dispatch(link_dispatch&&) = delete;

      ~link_dispatch() {};

      std::function<link_layer(const u_int8_t*)> operator[] (int key);

    };

    class link_parse_functions {
    public:

      link_parse_functions() = delete;

      static link_layer _EN10MB_parse(const u_int8_t*);

    };

  };

}


#endif
