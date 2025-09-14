//
// Created by jeremiah tesfaye on 8/31/25.
//

#include <layerx/layer3/IPv4.h>
#include <util/PacketRead.h>
#include <layerx/iana_numbers.h>
#include <format>
#include <utility>

IPv4::IPv4(size_t len, std::string src, std::string dest, u_int16_t frag_fields, u_int8_t protocol)
: NetworkPDU(len, std::move(src), std::move(dest))
, protocol(protocol)
, flags(frag_fields)
{
}

IPv4::~IPv4() = default;

std::string IPv4::make_info() const {

    using namespace layer;

    std::string info = std::format("{} -> {}", src, dest);

    switch (protocol) {

        case iana::ICMP:
            info += " ICMP";
            break;

        case iana::TCP:
            info += " TCP";
            break;

        case iana::UDP:
            info += " UDP";
            break;

        default:
    }

    if (flags & IP_MF) {
        info += " MF Fragmented";
    }

    return info;
}

std::string IPv4::name() const {
    return "IPv4";
}


std::unique_ptr<NetworkPDU> IPv4_functions::ipv4_parse(
    const std::vector<std::byte> &raw_data,
    packet::parse_context &context) {

    return {};

}

