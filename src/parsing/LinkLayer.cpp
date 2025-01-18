

#include "LinkLayer.h"

namespace Parse {

  LinkParse::LinkParse
  (
    u_int8_t& raw_data, 
    std::function<link_layer(u_int8_t&)> func,
    int link_type
  ) : raw_data(raw_data), layer_func(func)
  {

    link_dispatch link_function;
    layer_func = link_function[link_type];


  }

  LinkParse::link_dispatch::link_dispatch() {

    table[(std::hash<int>()(DLT_EN10MB) % DLT_TYPES)] = &link_parse_functions::_EN10MB_parse;

  }

  std::function<link_layer(u_int8_t&)> 
  LinkParse::link_dispatch::operator[] (int key) {

    int hash = std::hash<int>()(key) % DLT_TYPES;

    if (table[key]) {
      return table[key];
    }

  }

  

}