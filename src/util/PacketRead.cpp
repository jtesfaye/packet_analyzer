//
// Created by jeremiah tesfaye on 5/20/25.
//

#include <util/PacketRead.h>
#include <sstream>
#include <iomanip>
#include <vector>
#include <format>

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


std::string PacketRead::format_ipv4_src_dst(const u_int32_t& src_dest_add) {

    const u_int32_t addr = ntohl(src_dest_add);

    return std::format("{}.{}.{}.{}",
        addr >> 24 & 0xFF,
        addr >> 16 & 0xFF,
        addr >> 8 & 0xFF,
        addr & 0xFF
        );
}

std::string PacketRead::format_ipv6_src_dest(const u_int8_t *addr) {

    std::ostringstream oss;
    for (int i = 0; i < 16; i++) {

        // If i is greater than 0 and even.
        if (i > 0 && i & !(i & 1)) {
            oss << ":";
        }

        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr[i]);

    }

    return oss.str();
}


bool PacketRead::valid_length(
    const std::vector<std::byte> &data,
    const size_t offset,
    const size_t needed) {

    return data.size() >= offset + needed;
}





