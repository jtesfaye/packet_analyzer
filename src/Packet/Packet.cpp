
#include "Packet.h"
#include <iostream>
#include "../data_structures/DispatchTable.h"
#include "ProtocolTypes.h"
#include <iomanip>


namespace packet {

  Packet::Packet
  () 
  : m_dlt{0}, 
    m_flags{0}, 
    m_raw_data(nullptr), 
    m_meta_data(nullptr), 
    layer2_parse(nullptr)
  {}


  void
  Packet::init(int dlt, u_int8_t flags) {

    m_dlt = dlt;
    m_flags = flags;

    layer2_parse = std::make_unique<parse::LinkParse>(dlt);

  }



  packet_model
  Packet::start_extract() {

    packet_model pkt;

    layer2_parse->set_capture_data(m_meta_data);

    pkt.l2 = (*layer2_parse)(m_raw_data);

    set_layer2_display(pkt);

    if (m_flags & 0x40) {
      //call network parse using network type
    }

    if (m_flags & 0x20) {

    }

    return pkt;

  }



  void
  Packet::set_meta_data(const pcap_pkthdr* &m) {
    m_meta_data = m;
  }



  void
  Packet::set_raw_data(const u_int8_t* &raw_data) {
    m_raw_data = raw_data;
  }



  void
  Packet::set_layer2_display(packet_model& pkt) {


    DispatchTable<display_data::display_type, display_data::size> l2_functions(m_all_l2_display);
    pkt.l2_display = l2_functions[m_dlt];

  }


  
  void 
  Packet::display_data::show_802_11
  (const link_layer& l2) {

    using namespace packet::frame;
    namespace mask = packet::frame::_802_11_mask;

    const frame::_802_11* data = std::get<const frame::_802_11*>(l2);
    
    std::cout << "Layer 2 data: ";
    std::cout << "Frame control [ ";
    std::cout << "Protocol version: " << (ntohs(data->frame_control) & mask::PROTOCOL_VERS);
    std::cout << "Type: " << (ntohs(data->frame_control) & mask::TYPE);
    std::cout << "Protected Frame: " << (ntohs(data->frame_control) & mask::PROTECTED);
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

  void
  Packet::display_data::show_EN10MB
  (const link_layer& l2) {

    if (const frame::EN10MB* data = *(std::get_if<const frame::EN10MB*>(&l2))) {
      
      std::cout << "Destination: ";
      for (size_t i = 0; i < 6; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
          << static_cast<int>(data->dest_addr[i]);
        if (i < 5) {
          std::cout << ":";
        } 
      }
      std::cout << " Source: ";

      for (size_t i = 0; i < 6; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') <<
          static_cast<int>(data->src_addr[i]);
        if (i < 5) {
          std::cout << ":";
        } 
      }
      std::cout << "\n";
    } else {
      std::cout << "not EN10mb\n"; 
    }

  } 

}