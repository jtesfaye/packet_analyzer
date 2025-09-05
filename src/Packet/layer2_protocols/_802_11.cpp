//
// Created by jeremiah tesfaye on 7/27/25.
//

#include "tests/include/layer2_protocols/_802_11.h"


#include "tests/include/packet/ProtocolTypes.h"

link_layer_ref _802_11_functions::_802_11_parse(const u_int8_t* raw_data, parse_context& context) {

  namespace mask = _802_11_mask;

  bpf_u_int32 packet_len = context.header->caplen;

  if (sizeof(_802_11) > packet_len) {

    context.layer3_type = ip::NT_UNSUPPORTED;
    context.length = 0;
    return link_layer_ref {};

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

      context.layer3_type = ip::NT_UNSUPPORTED;
      return link_layer_ref {};

    }

    const _802_2* llc_header = reinterpret_cast<const _802_2*>(raw_data + payload_start);

    u_int8_t control_offset = llc_header->control & 0x01 ? 1 : 0;


    if (llc_header->DSAP_addr == 0xAA) {


      if (payload_start + sizeof(_802_2) + sizeof(snap_extension) > packet_len) {

        //malformed snap header
        context.layer3_type = ip::NT_UNSUPPORTED;
        context.length = 0;
        return link_layer_ref {};

      }

      const snap_extension* snap
        = reinterpret_cast<const snap_extension*> (llc_header + control_offset);


      if (snap->oui[0] == 0x00 && snap->oui[1] == 0x00 && snap->oui[2] == 0x00) {

        context.layer3_type = snap->protocol_id;
        context.length = payload_start + sizeof(_802_2) + control_offset + sizeof(snap_extension);

      }


    } else {

      context.layer3_type = ip::NT_UNSUPPORTED;
      context.length = 0;
      return link_layer_ref {};

    }

  } else if ((frame_control & mask::TYPE) == mask::MANAGMENT ||
             (frame_control & mask::TYPE) == mask::CONTROL) {

    context.layer3_type = ip::NT_UNSUPPORTED;
    context.length = 0;
    return link_layer_ref {const_cast<_802_11*>(frame)};

  }

  return link_layer_ref {const_cast<_802_11*>(frame)};

}

