//
// Created by jeremiah tesfaye on 9/18/25.
//

#include <layerx/layer3/IPv6.h>
#include <layerx/iana_numbers.h>
#include <utility>
#include <util/PacketRead.h>

IPv6::IPv6(size_t len, std::string src, std::string dest, u_int8_t protocol)
: NetworkPDU(len, std::move(src), std::move(dest))
, protocol(protocol) {

}

std::string IPv6::make_info() const {

    using namespace layer;

    return iana::protocol_to_string(protocol);
}

std::string IPv6::name() const {
    return "IPv6";
}

Layer3Registry &IPv6_functions::get_ipv6_registry() {
    static Layer3Registry ipv6_reg(layer::iana::IPV6, ipv6_parse);
    return ipv6_reg;
}

std::unique_ptr<NetworkPDU> IPv6_functions::ipv6_parse(
    const std::vector<std::byte>& raw_data,
    packet::parse_context& context) {
    size_t start = context.offset;

    if (!PacketRead::valid_length(raw_data, start, sizeof(ipv6_header))) {
        return nullptr;
    }

    const auto ipv6_hdr = reinterpret_cast<const ipv6_header*> (raw_data.data() + start);

    if (u_int8_t version = ipv6_version(ipv6_hdr); version != 6) {
        return nullptr;
    }

    size_t next_protocol = ipv6_hdr->next_header;

    context.curr_length = sizeof(ipv6_header);
    context.next_type = next_protocol;

    return std::make_unique<IPv6>(
        context.curr_length,
        PacketRead::format_ipv6_src_dest(ipv6_hdr->src_addr),
        PacketRead::format_ipv6_src_dest(ipv6_hdr->dst_addr),
        next_protocol
    );
}





