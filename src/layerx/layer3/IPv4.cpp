//
// Created by jeremiah tesfaye on 8/31/25.
//

#include <layerx/layer3/IPv4.h>
#include <util/PacketRead.h>
#include <layerx/iana_numbers.h>
#include <format>
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

    info += iana::protocol_to_string(protocol);

    return info;
}

std::string IPv4::name() const {
    return "IPv4";
}

void IPv4_functions::register_ipv4() {
    static Layer3Registry ipv4_reg(layer::iana::IPV4, ipv4_parse);
    static Layer3Registry ipv4_detail_reg(layer::iana::IPV4, ipv4_detailed_parse);
}

std::unique_ptr<NetworkPDU> IPv4_functions::ipv4_parse(
    const std::vector<std::byte> &raw_data,
    packet::parse_context &context) {

    size_t start = context.offset;

    if (!PacketRead::valid_length(raw_data, start, sizeof(ipv4_header))) {
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
        PacketRead::format_ipv4_src_dst(ipv4_hdr->src_addr),
        PacketRead::format_ipv4_src_dst(ipv4_hdr->dest_adr),
        is_fragmented,
        next_protocol);
}

ProtocolDetails IPv4_functions::ipv4_detailed_parse(
    const std::vector<std::byte>& raw_data,
    parse_context& context) {

    using namespace layer;

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

    std::string src = PacketRead::format_ipv4_src_dst(hdr->src_addr);
    std::string dst = PacketRead::format_ipv4_src_dst(hdr->dest_adr);

    details.push_back(std::format("Version: {}", version));
    details.push_back(std::format("Header Length: {} bytes", ihl * 4));
    details.push_back(std::format("DSCP: 0x{:02X}", dscp));
    details.push_back(std::format("ECN: 0x{:02X}", ecn));
    details.push_back(std::format("Total Length: {}", total_length));
    details.push_back(std::format("Identification: {}", identification));
    details.push_back(std::format("Flags: 0b{:03b}", flags));
    details.push_back(std::format("Fragment Offset: {}", frag_offset));
    details.push_back(std::format("Time to Live (TTL): {}", ttl));
    details.push_back(std::format("Protocol: {} ({})", protocol, iana::protocol_to_string(protocol)));
    details.push_back(std::format("Header Checksum: 0x{:04X}", checksum));
    details.push_back(std::format("Source Address: {}", src));
    details.push_back(std::format("Destination Address: {}", dst));

    return {full_protocol_name(),details};

}
