
#include "Packet.h"

using std::cout;


namespace packet {

  Packet::Packet
  (
  const int dlt, 
  const u_int8_t flags, 
  const u_int8_t* &raw_data) 
  : m_dlt(dlt), m_flags(flags), m_raw_data(raw_data), getLayer2(dlt)
  {}



  void
  Packet::start_extract() {

    m_data.l2 = getLayer2(m_raw_data);

    if (m_flags & 0x40) {
      //call network parse using network type
    }

  }
  
  void 
  Packet::display_data::show_802_11
  (const link_layer& l2) {

    const frame::_802_11* data = std::get<const frame::_802_11*>(l2);
    
    std::cout << "Layer 2 data: ";
    std::cout << "Frame control [ ";
    std::cout << "Protocol version: " << (ntohs(data->frame_control) & frame::_802_11_PVRSN);
    std::cout << "Type: " << (ntohs(data->frame_control) & frame::_802_11_TYPE);
    std::cout << "Protected Frame: " << (ntohs(data->frame_control) & frame::_802_11_PROTECTED);
    std::cout << "]";

    std::cout << "SOURCE: [";
    for (int i = 0; i < 6; i++) {
      std::cout << data->addr1[i];
      std::cout << ".";
    }
    std::cout << "]\n";

    std::cout << "Destination: [";
    for (int i = 0; i < 6; i++) {
      std::cout << data->addr2[i];
      std::cout << ".";
    }
    std::cout << "]\n";

  }

}