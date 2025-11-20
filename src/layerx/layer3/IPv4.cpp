//
// Created by jeremiah tesfaye on 8/31/25.
//

#include <layerx/layer3/IPv4.h>
#include <layerx/layer3/Layer3Registry.h>
#include <format>
#include <utility>

IPv4::IPv4(const size_t len, const u_int32_t src, const u_int32_t dest, const bool is_fragmented, const u_int8_t protocol)
: NetworkPDU(len)
, protocol(protocol)
, is_fragmented(is_fragmented)
{
    using namespace protocol::ipv4;
    std::memcpy(&src_address.bytes, &src, addr_len);
    std::memcpy(&dest_address.bytes, &dest, addr_len);
    src_address.size = addr_len;
    dest_address.size = addr_len;
}

IPv4::~IPv4() = default;

std::string IPv4::make_info() const {

    std::string info;

    if (is_fragmented) {
        info += "(Fragmented datagram) ";
    }

    info += packet::protocol_to_string(protocol);

    return info;
}

std::string_view IPv4::name() const {
    return protocol::ipv4::name;
}

std::string IPv4::address_to_string(const Address &addr) const {

    u_int32_t target{};
    std::memcpy(&target, &addr.bytes, sizeof(target));
    return packet::format_ipv4_src_dst(target);
}

Address IPv4::src() const {
    return src_address;
}

Address IPv4::dest() const {
    return dest_address;
}

void protocol::ipv4::register_ipv4() {
    registry::layer3::register_self(iana_number, ipv4_parse);
    registry::layer3::register_self(iana_number, ipv4_detailed_parse);
}

std::unique_ptr<NetworkPDU> protocol::ipv4::ipv4_parse(
    std::span<std::byte> raw_data,
    packet::parse_context &context) {

    size_t start = context.offset;

    if (!valid_length(raw_data, start, sizeof(ipv4_header))) {
        return nullptr;
    }

    const auto ipv4_hdr = reinterpret_cast<const ipv4_header*> (raw_data.data() + start);

    //If not equal probably means we have malformed data or misaligned data
    if (u_int8_t version = (ipv4_hdr->version_ihl >> 4) & 0x0F; version != 4) {
        return nullptr;
    }

    bool is_fragmented = false;

    const u_int16_t frag_field = ntohs(ipv4_hdr->flags_foffset);
    const u_int8_t flags = (frag_field >> 13) & 0x07;

    //more fragment flag is set
    if (u_int16_t offset = frag_field & 0x1FFF; (flags & 0x01) || (offset > 0)) {
        is_fragmented = true;
        context.is_fragmented = true;
    }

    size_t header_len = (ipv4_hdr->version_ihl >> 4) * 5;
    size_t next_protocol = ipv4_hdr->protocol;

    context.curr_length = header_len;
    context.next_type = next_protocol;

    return std::make_unique<IPv4>(
        header_len,
        ipv4_hdr->src_addr,
        ipv4_hdr->dest_adr,
        is_fragmented,
        next_protocol);
}

packet::ProtocolDetails protocol::ipv4::ipv4_detailed_parse(
    std::span<std::byte> raw_data,
    parse_context& context) {

    const auto* hdr = reinterpret_cast<const ipv4_header*>(raw_data.data() + context.offset);

    std::vector<std::string> details;
    details.reserve(10);

    uint8_t version = (hdr->version_ihl >> 4) & 0x0F;
    uint8_t ihl = hdr->version_ihl & 0x0F;
    uint8_t dscp = hdr->dscp_ecn >> 2;
    u_int8_t ecn = hdr->dscp_ecn & 0x03;
    uint16_t total_length = ntohs(hdr->length);
    uint16_t identification = ntohs(hdr->id);
    uint16_t frag_field = ntohs(hdr->flags_foffset);
    uint8_t flags = (frag_field >> 13) & 0x07;
    uint16_t frag_offset = frag_field & 0x1FFF;
    uint8_t ttl = hdr->ttl;
    uint8_t protocol = hdr->protocol;
    uint16_t checksum = ntohs(hdr->chksum);

    std::string src = format_ipv4_src_dst(hdr->src_addr);
    std::string dst = format_ipv4_src_dst(hdr->dest_adr);

    details.push_back(std::format("Version: {}", version));
    details.push_back(std::format("Header Length: {} bytes", ihl * 4));
    details.push_back(std::format("DSCP: 0x{:02X}", dscp));
    details.push_back(std::format("ECN: 0x{:02X}", ecn));
    details.push_back(std::format("Total Length: {}", total_length));
    details.push_back(std::format("Identification: {}", identification));
    details.push_back(std::format("Flags: 0b{:03b}", flags));
    details.push_back(std::format("Fragment Offset: {}", frag_offset));
    details.push_back(std::format("Time to Live (TTL): {}", ttl));
    details.push_back(std::format("Protocol: {} ({})", protocol, protocol_to_string(protocol)));
    details.push_back(std::format("Header Checksum: 0x{:04X}", checksum));
    details.push_back(std::format("Source Address: {}", src));
    details.push_back(std::format("Destination Address: {}", dst));

    return {full_protocol_name,details};
}
