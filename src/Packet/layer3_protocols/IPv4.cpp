//
// Created by jeremiah tesfaye on 8/31/25.
//

#include <layer3_protocols/IPv4.h>
#include <util/PacketRead.h>
#include <format>

#include <utility>

IPv4::IPv4(size_t len, std::string src, std::string dest, u_int16_t frag_fields, u_int8_t protocol)
: NetworkPDU(len, std::move(src), std::move(dest))
, protocol(protocol)
, flags(frag_fields)
{
}

std::string IPv4::make_info() const {

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


std::unique_ptr<IPv4> IPv4_functions::IPv4_parse(
    const std::vector<std::byte> &raw_data,
    packet::parse_context &context) {

    using namespace layer::ip;

    if (raw_data.size() < sizeof(ipv4_header)) {
        throw std::runtime_error("Packet is too small to contain ipv4 header");
    }

    const std::byte* start = raw_data.data() + context.offset;

    const auto ipv4_hdr = reinterpret_cast<const ipv4_header*> (start);

    packet::row_entry& entry = context.entry;

    context.length = (ipv4_hdr->version_ihl & 0x0F) * 4;
    context.next_type = ipv4_hdr->protocol;

    const auto* src_start = reinterpret_cast<const std::byte*>(&ipv4_hdr->src_addr);
    const auto* dest_start = reinterpret_cast<const std::byte*> (&ipv4_hdr->dest_adr);

    entry.src = ipv4_src_dest_format(src_start);
    entry.dest = ipv4_src_dest_format(dest_start);

    return net_layer_ref{static_cast<ipv4_header*>(nullptr)};

    //ihl tells us if its full length
    //need to check if its fragmented

}

