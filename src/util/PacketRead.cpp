//
// Created by jeremiah tesfaye on 5/20/25.
//



#include <util/PacketRead.h>
#include <sstream>
#include <iomanip>

//We are working on making linkread generic, and now trying to implement into this func

PacketRead::PacketRead() = default;

std::string
PacketRead::format_mac(const u_int8_t *addr) {

    std::ostringstream oss;
    for (int i = 0; i < 6; ++i) {
        if (i > 0)
            oss << ":";
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr[i]);
    }
    return oss.str();
}


std::string PacketRead::format_ipv4_src_dst(const std::byte *addr) {

    std::ostringstream oss;
    for (int i = 0; i < 4; ++i) {
        if (i > 0)
            oss << ".";
        oss << std::hex << static_cast<int>(addr[i]);
    }

    return oss.str();
}




