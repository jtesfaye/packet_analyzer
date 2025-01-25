

#include "LinkLayer.h"
#include <cstring>

namespace Parse {

  LinkParse::LinkParse
  (
    int link_type
  )
  {

    link_dispatch link_function;
    layer_func = link_function[link_type];

    if (!layer_func) {
      std::cerr << "Unsupported datalink type" << std::endl;
      throw std::runtime_error("Unsuported data link type");
    }

  }

  LinkParse::link_dispatch::link_dispatch() {

    table[(std::hash<int>()(DLT_EN10MB) % DLT_TYPES)] 
    = &link_parse_functions::_EN10MB_parse;

  }

  std::function<link_layer(const u_int8_t*)> 
  LinkParse::link_dispatch::operator[] 
  (int key) {

    int hash = std::hash<int>()(key) % DLT_TYPES;

    if (table[key]) {
      return table[key];
    }

  }

  link_layer
  LinkParse::link_parse_functions::_EN10MB_parse
  (const u_int8_t* raw_data) {

    const EN10MB* ether = reinterpret_cast<const EN10MB*> (raw_data);

    if (ntohs(ether->ether_type) == 0x8100) {

      const EN10MB_802_1_Q* vlan = reinterpret_cast< const EN10MB_802_1_Q*> (raw_data);

      link_layer frame;
      frame.vlan_ether_frame = vlan;

      return  frame;
    }

    link_layer frame;
    frame.ether_frame = ether;

    return frame;

  }

}