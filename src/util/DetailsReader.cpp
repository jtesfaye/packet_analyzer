//
// Created by jeremiah tesfaye on 7/5/25.
//

#include <util/DetailsReader.h>
#include <parsing/PacketRead.h>
#include <sstream>

std::string DetailsReader::operator()(EN10MB& pkt) const {

    std::ostringstream oss;

    const std::string src = PacketRead::format_mac(pkt.src_addr);
    const std::string dest = PacketRead::format_mac(pkt.dest_addr);

    oss << "Ethernet:\n";
    oss << "    Source IP: " << src << "\n";
    oss << "    Destination IP: " << dest << "\n";
    oss << "    Ether Type: 0x" << std::hex << pkt.ether_type << std::dec << "\n";
    return oss.str();


}
