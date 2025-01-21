

#include "LinkLayer.h"

namespace Parse {

  LinkParse::LinkParse
  (
    u_int8_t& raw_data, 
    int link_type
  ) : raw_data(raw_data)
  {

    link_dispatch link_function;
    layer_func = link_function[link_type];

  }

  LinkParse::link_dispatch::link_dispatch() {

    table[(std::hash<int>()(DLT_EN10MB) % DLT_TYPES)] 
    = &link_parse_functions::_EN10MB_parse;

  }

  std::function<link_layer(u_int8_t&)> 
  LinkParse::link_dispatch::operator[] 
  (int key) {

    int hash = std::hash<int>()(key) % DLT_TYPES;

    if (table[key]) {
      return table[key];
    }

  }

  link_layer
  LinkParse::link_parse_functions::_EN10MB_parse
  (const u_int8_t& raw_data) {

    link_layer frame;

    EN10MB* ether = reinterpret_cast<EN10MB*> (raw_data);

    if (ntohs(ether->ether_type) == 0x8100) {

      EN10MB_802_1_Q* vlan = reinterpret_cast<EN10MB_802_1_Q*> (raw_data);
      frame.vlan_ether_frame = vlan;
      
      return frame;
    }

    frame.ether_frame = ether;

    return frame;

  }

}