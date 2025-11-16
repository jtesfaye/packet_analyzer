//
// Created by jeremiah tesfaye on 5/20/25.
//

#include <util/PacketRead.h>
#include <sstream>
#include <iomanip>
#include <vector>
#include <format>
#include <span>

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
    std::array<uint16_t, 8> segments{};

    // Convert bytes to 16-bit segments
    for (int i = 0; i < 16; i += 2) {
        segments[i / 2] = (addr[i] << 8) | addr[i + 1];
    }

    // Find the longest run of zeros
    int bestStart = -1, bestLen = 0;
    for (int i = 0; i < 8; ) {
        if (segments[i] == 0) {
            int j = i;
            while (j < 8 && segments[j] == 0) j++;
            int len = j - i;
            if (len > bestLen) {
                bestLen = len;
                bestStart = i;
            }
            i = j;
        } else {
            i++;
        }
    }

    // Don’t compress if the run is only one group
    if (bestLen < 2) {
        bestStart = -1;
        bestLen = 0;
    }

    // Build the string
    std::ostringstream oss;
    oss << std::hex;
    for (int i = 0; i < 8; ) {
        if (i == bestStart) {
            oss << "::";
            i += bestLen;
            if (i >= 8)
                break;
        } else {
            if (i > 0 && i != bestStart + bestLen)
                oss << ":";
            oss << std::noshowbase << std::hex << std::nouppercase << segments[i];
            i++;
        }
    }

    return oss.str();
}


bool PacketRead::valid_length(
    std::span<std::byte> data,
    const size_t offset,
    const size_t needed) {

    return data.size() >= offset + needed;
}





