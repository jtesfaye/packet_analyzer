

#include "LinkLayer.h"
#include <cstring>

namespace parse {

  LinkParse::LinkParse
  (
    int link_type
  )
  {

    link_dispatch link_function(*this);
    layer_func = link_function[link_type];

    if (!layer_func) {
      std::cerr << "Unsupported datalink type" << std::endl;
      throw std::runtime_error("Unsuported data link type");
    }

  }



  u_int16_t
  LinkParse::layer3_type() {
    return m_layer3_type;
  }


  
  u_int8_t
  LinkParse::header_length() {
    return m_header_length;
  }



  void 
  LinkParse::set_layer3_type(u_int16_t type) {

    switch (type) {

      case 0x0800:
        m_layer3_type = ip::NT_IPV4;
        break;
      
      case 0x86DD:
        m_layer3_type = ip::NT_IPV6;
        break;

      default:
        m_layer3_type = ip::NT_UNSUPPORTED;
        break;
      }
  }



  void
  LinkParse::set_length(u_int8_t len) {
    m_header_length = len;
  }



  LinkParse::link_dispatch::link_dispatch(LinkParse&) {

    table[(std::hash<int>()(DLT_EN10MB) % DLT_TYPES)] 
    = &link_parse_functions::_EN10MB_parse;

    table[(std::hash<int>()(DLT_IEEE802_11) % DLT_TYPES)]
    = &link_parse_functions::_802_11_parse;

  }



  std::function<link_layer(const u_int8_t*, LinkParse&)> 
  LinkParse::link_dispatch::operator[] 
  (int key) {

    int hash = std::hash<int>()(key) % DLT_TYPES;

    if (table[key]) {
      return table[key];
    }

  }



  link_layer
  LinkParse::link_parse_functions::_EN10MB_parse
  (const u_int8_t* raw_data, LinkParse& link) {

    const EN10MB* ether = reinterpret_cast<const EN10MB*> (raw_data);

    if (ntohs(ether->ether_type) == 0x8100) {

      const EN10MB_802_1_Q* vlan = reinterpret_cast< const EN10MB_802_1_Q*> (raw_data);

      link.set_length(18);
      link.set_layer3_type(ntohs(vlan->ether_type));

      return  link_layer {vlan};
    }

    link.set_length(14);
    link.set_layer3_type(ntohs(ether->ether_type));

    return link_layer {ether};

  }



  link_layer
  LinkParse::link_parse_functions::_802_11_parse
  (const u_int8_t* raw_data, LinkParse& link) {

    const _802_11* frame = reinterpret_cast<const _802_11*> (raw_data);

    using mask = _802_11_Masks;

    u_int16_t frame_control = ntohs(frame->frame_control);

    if ((frame_control & mask::TYPE) == mask::DATAFRAME) {

      //determine where the payload starts, integer represents how many bytes from the start of the header is the payload
      u_int8_t payload_start = 18;

      //if ToDS and FromDs is present, addr 4 is present in frame
      if (frame_control & mask::ToDS && (frame->frame_control & mask::FromDS))
        payload_start += 6; 

      //subtype between 8 and 15 indicate QoS field is present
      u_int8_t subtype = frame_control & mask::SUBTYPE;

      if (subtype > 7 &&  subtype < 16) {
        payload_start += 2;

        if (frame_control & mask::ORDER)
          payload_start += 4;

      }

      const _802_2* llc_header = reinterpret_cast<const _802_2*>(raw_data + payload_start);

      u_int8_t control_offset = llc_header->control & 0x01 ? 1 : 0;

      if (llc_header->DSAP_addr == 0xAA) {
        
        const snap_extension* snap 
          = reinterpret_cast<const snap_extension*> (llc_header + control_offset);

        u_int8_t id[3];
        bool is_ether_type {true};

        std::memcpy(id, snap->oui, 3);

        for (int i = 0; i < 3; i++) {

          if (id[i] != 0x00) {
            is_ether_type = false;
            break;
          }
        }

        if (is_ether_type) {
          
        }

        

      }

      
        

    } else {

      link.data_frame = false;

    }

    return link_layer {frame};

  }

}