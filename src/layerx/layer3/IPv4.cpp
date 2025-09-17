//
// Created by jeremiah tesfaye on 8/31/25.
//

#include <layerx/layer3/IPv4.h>
#include <util/PacketRead.h>
#include <layerx/iana_numbers.h>
#include <format>
#include <iostream>
#include <utility>

IPv4::IPv4(size_t len, std::string src, std::string dest, const bool is_fragmented, u_int8_t protocol)
: NetworkPDU(len, std::move(src), std::move(dest))
, protocol(protocol)
, is_fragmented(is_fragmented)
{
}

IPv4::~IPv4() = default;

std::string IPv4::make_info() const {

    using namespace layer;

    std::string info;

    if (is_fragmented) {
        info += "(Fragmented datagram) ";
    }

    switch (protocol) {

        case iana::ICMP:
            info += "ICMP ";
            break;

        case iana::TCP:
            info += "TCP ";
            break;

        case iana::UDP:
            info += "UDP ";
            break;

        default:
    }

    return info;
}

std::string IPv4::name() const {
    return "IPv4";
}

std::unique_ptr<NetworkPDU> IPv4_functions::ipv4_parse(
    const std::vector<std::byte> &raw_data,
    packet::parse_context &context) {

    size_t start = context.offset;

    if (!PacketRead::valid_length(raw_data, start, sizeof(ipv4_header))) {
        std::cout << "not valid length\n";
        return nullptr;
    }

    const auto ipv4_hdr = reinterpret_cast<const ipv4_header*> (raw_data.data() + start);

    //If not equal probably means we have malformed data or misaligned data
    if (u_int8_t version = (ipv4_hdr->version_ihl >> 4) & 0x0F; version != 4) {
        std::cout << version << std::endl;
        std::cout << "misaligned\n";
        return nullptr;
    }

    bool is_fragmented = false;

    const u_int16_t frag_field = ntohs(ipv4_hdr->flags_foffset);
    const u_int8_t flags = (frag_field >> 13) & 0x07;

    //more fragment flag is set
    if (u_int16_t offset = frag_field & 0x1FFF; (flags & 0x01) || (offset > 0)) {
        is_fragmented = true;
    }

    size_t header_len = (ipv4_hdr->version_ihl >> 4) * 5;
    size_t next_protocol = ipv4_hdr->protocol;

    context.curr_length = header_len;
    context.next_type = next_protocol;

    return std::make_unique<IPv4>(
        header_len,
        PacketRead::format_ipv4_src_dst(ipv4_hdr->src_addr),
        PacketRead::format_ipv4_src_dst(ipv4_hdr->dest_adr),
        is_fragmented,
        next_protocol);
}

