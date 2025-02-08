

#include "LinkLayer.h"
#include <cstring>
#include "../data_structures/DispatchTable.h"
#include <iostream>


namespace parse {

  LinkParse::LinkParse
  (
    int link_type
  ) : m_capture_data(nullptr)
  {
    

    DispatchTable
    <link_layer(const u_int8_t*, LinkParse&), link_parse_functions::total> 
    link_table(all_parse_functions);

    layer_func = link_table[link_type];

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
  LinkParse::set_capture_data
  (const pcap_pkthdr* &header) {
    m_capture_data = header;
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

    namespace mask = packet::frame::_802_11_mask;

    bpf_u_int32 packet_len = link.m_capture_data->caplen;

    if (sizeof(_802_11) > packet_len) {

      link.set_layer3_type(ip::NT_UNSUPPORTED);
      link.set_length(0);
      return link_layer {};

    }

    const _802_11* frame = reinterpret_cast<const _802_11*> (raw_data);

    u_int16_t frame_control = ntohs(frame->frame_control);

    if ((frame_control & mask::TYPE) == mask::DATAFRAME) {

      //determine where the payload starts, integer represents how many bytes from the start of the header is the payload
      u_int8_t payload_start = 24;

      //if ToDS and FromDs is present,then addr 4 is present in frame
      if (frame_control & mask::ToDS && (frame->frame_control & mask::FromDS))
        payload_start += 6; 

      //subtype between 8 and 15 indicate QoS field is present
      u_int8_t subtype = frame_control & mask::SUBTYPE;

      if (subtype > 7 &&  subtype < 16) {

        payload_start += 2;

        if (frame_control & mask::ORDER)
          payload_start += 4;

      }

      if (payload_start + sizeof(_802_2) > packet_len)  { //check if data is within bounds of packet

        link.set_layer3_type(ip::NT_UNSUPPORTED);
        return link_layer {};

      }

      const _802_2* llc_header = reinterpret_cast<const _802_2*>(raw_data + payload_start);

      u_int8_t control_offset = llc_header->control & 0x01 ? 1 : 0;


      if (llc_header->DSAP_addr == 0xAA) {


        if (payload_start + sizeof(_802_2) + sizeof(snap_extension) > packet_len) {

          //malformed snap header
          link.set_layer3_type(ip::NT_UNSUPPORTED);
          link.set_length(0);
          return link_layer {};

        }
        
        const snap_extension* snap 
          = reinterpret_cast<const snap_extension*> (llc_header + control_offset);


        if (snap->oui[0] == 0x00 && snap->oui[1] == 0x00 && snap->oui[2] == 0x00) {
          
          link.set_layer3_type(snap->protocol_id);
          link.set_length(payload_start + sizeof(_802_2) + control_offset + sizeof(snap_extension));

        }


      } else {

        link.set_layer3_type(ip::NT_UNSUPPORTED);
        link.set_length(0);
        return link_layer {};

      }

    } else if ((frame_control & mask::TYPE) == mask::MANAGMENT ||
               (frame_control & mask::TYPE) == mask::CONTROL) {

      link.set_layer3_type(ip::NT_UNSUPPORTED);
      link.set_length(0);
      return link_layer {frame};

    } 

    return link_layer {frame};

  }

}