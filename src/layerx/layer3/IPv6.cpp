//
// Created by jeremiah tesfaye on 9/18/25.
//

#include <layerx/layer3/IPv6.h>
#include <layerx/layer3/Layer3Registry.h>
#include <format>
IPv6::IPv6(const size_t len, const u_int8_t *src, const u_int8_t *dest, const u_int8_t protocol)
: NetworkPDU(len)
, protocol(protocol) {

    std::memcpy(src_address.bytes.data(), src, protocol::ipv6::addr_len);
    std::memcpy(dest_address.bytes.data(), dest, protocol::ipv6::addr_len);
    src_address.size = protocol::ipv6::addr_len;
    dest_address.size = protocol::ipv6::addr_len;
}

std::string IPv6::make_info() const {

    return packet::protocol_to_string(protocol);
}

std::string_view IPv6::name() const {
    return protocol::ipv6::name;
}

std::string IPv6::address_to_string(const Address &addr) const {

    u_int8_t data[16];
    std::memcpy(&data, addr.bytes.data(), 16);
    return packet::format_ipv6_src_dest(data);
}

Address IPv6::src() const {
    return src_address;
}

Address IPv6::dest() const {
    return dest_address;
}

void protocol::ipv6::register_ipv6() {
    registry::layer3::register_self(iana_number, ipv6_parse);
    registry::layer3::register_self(iana_number, ipv6_detailed_parse);
}

std::unique_ptr<NetworkPDU> protocol::ipv6::ipv6_parse(
    std::span<std::byte> raw_data,
    parse_context& context) {
    size_t start = context.offset;

    if (!valid_length(raw_data, start, sizeof(ipv6_header))) {
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
        ipv6_hdr->src_addr,
        ipv6_hdr->dst_addr,
        next_protocol
    );
}

packet::ProtocolDetails protocol::ipv6::ipv6_detailed_parse(
    std::span<std::byte> raw_data,
    parse_context &context) {

    using namespace std;

    const auto* hdr = reinterpret_cast<const ipv6_header*>(raw_data.data() + context.offset);

    vector<string> details;
    details.reserve(8);

    uint8_t version = ipv6_version(hdr);
    uint8_t traffic_class = ipv6_traffic_class(hdr);
    uint32_t flow_label = ipv6_flow_label(hdr);
    uint16_t payload_length = ntohs(hdr->payload_length);
    uint8_t next_header = hdr->next_header;
    uint8_t hop_limit = hdr->hop_limit;

    string src = format_ipv6_src_dest(hdr->src_addr);
    string dst = format_ipv6_src_dest(hdr->dst_addr);

    details.push_back(std::format("Version: {}", version));
    details.push_back(std::format("Traffic Class: 0x{:02X}", traffic_class));
    details.push_back(std::format("Flow Label: 0x{:05X}", flow_label));
    details.push_back(std::format("Payload Length: {}", payload_length));
    details.push_back(std::format("Next Header: {} ({})", next_header, protocol_to_string(next_header)));
    details.push_back(std::format("Hop Limit: {}", hop_limit));
    details.push_back(std::format("Source Address: {}", src));
    details.push_back(std::format("Destination Address: {}", dst));

    return { full_protocol_name, details };
}





