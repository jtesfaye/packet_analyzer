//
// Created by jeremiah tesfaye on 9/18/25.
//

#include <packet/PacketUtil.h>
#include <string>
#include <format>
#include <iomanip>

using namespace packet;
packet_ref::~packet_ref() = default;

packet_ref::packet_ref() = default;

packet_ref::packet_ref(packet_ref&&) noexcept = default;

packet_ref& packet_ref:: operator=(packet_ref&&) noexcept = default;

std::string packet::format_mac(const u_int8_t *addr) {

    std::ostringstream oss;
    for (int i = 0; i < 6; ++i) {
        if (i > 0)
            oss << ":";
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr[i]);
    }
    return oss.str();
}


std::string packet::format_ipv4_src_dst(const u_int32_t& src_dest_add) {

    const u_int32_t addr = ntohl(src_dest_add);

    return std::format("{}.{}.{}.{}",
        addr >> 24 & 0xFF,
        addr >> 16 & 0xFF,
        addr >> 8 & 0xFF,
        addr & 0xFF
        );
}

std::string packet::format_ipv6_src_dest(const u_int8_t *addr) {
    std::array<uint16_t, 8> segments{};

    for (int i = 0; i < 16; i += 2) {
        segments[i / 2] = (addr[i] << 8) | addr[i + 1];
    }

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

    if (bestLen < 2) {
        bestStart = -1;
        bestLen = 0;
    }

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


bool packet::valid_length(
    std::span<std::byte> data,
    const size_t offset,
    const size_t needed) {

    return data.size() >= offset + needed;
}

std::string packet::protocol_to_string(u_int16_t protocol) {

    std::string info;

    switch (protocol) {

        case 1:
            info += "ICMP ";
        break;

        case 6:
            info += "TCP ";
        break;

        case 17:
            info += "UDP ";
        break;

        default:
    }

    return info;
}
