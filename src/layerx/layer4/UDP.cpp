//
// Created by jeremiah tesfaye on 10/2/25.
//

#include <layerx/layer4/UDP.h>
#include <format>
#include <layerx/layer4/Layer4Registry.h>

void protocol::udp::register_udp() {
    registry::layer4::register_self(iana_number, udp_parse);
    registry::layer4::register_self(iana_number, udp_detailed_parse);
}

UDP::UDP(const size_t len, const u_int16_t src_port, const u_int16_t dest_port)
: TransportPDU(len, src_port, dest_port)
{}

UDP::~UDP() = default;


std::string UDP::make_info() const {
    return "";
}

std::string_view UDP::name() const {
    return protocol::udp::name;
}

std::unique_ptr<TransportPDU> protocol::udp::udp_parse(
    std::span<std::byte> raw_data,
    parse_context &context) {

    if (valid_length(raw_data, context.offset, sizeof(udp_header))) {
        return nullptr;
    }

    const std::byte* start = raw_data.data() + context.offset;

    const auto udp_hdr = reinterpret_cast<const udp_header*> (start);

    size_t length = ntohs(udp_hdr->len);

    return std::make_unique<UDP>(
        length,
        ntohs(udp_hdr->src),
        ntohs(udp_hdr->dest)
        );
}

ProtocolDetails protocol::udp::udp_detailed_parse(
    std::span<std::byte> raw_data,
    parse_context &context) {

    const auto hdr = reinterpret_cast<const udp_header*> (raw_data.data() + context.offset);

    std::vector<std::string> details;
    details.reserve(5);

    details.emplace_back(std::format("Source: {}", ntohs(hdr->src)));
    details.emplace_back(std::format("Destination: {}", ntohs(hdr->dest)));
    details.emplace_back(std::format("Length: {}", ntohs(hdr->len)));
    details.emplace_back("Checksum: " + std::format(":x", ntohs(hdr->checksum)));
    details.emplace_back(std::format("UDP Payload ({})", std::to_string(ntohs(hdr->len) - 8)));

    return { full_protocol_name, details };
}

