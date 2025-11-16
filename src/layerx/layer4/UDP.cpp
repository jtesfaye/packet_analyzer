//
// Created by jeremiah tesfaye on 10/2/25.
//

#include <layerx/layer4/UDP.h>
#include <format>
#include <iostream>
#include <util/PacketRead.h>
#include <layerx/iana_numbers.h>

Layer4Registry& udp_functions::get_udp_registry() {
    static Layer4Registry udp_reg(layer::iana::UDP, udp_parse);
    static Layer4Registry udp_detail_reg(layer::iana::UDP, udp_detailed_parse);
    return udp_reg;
}

UDP::UDP(const size_t len, std::string src_port, std::string dest_port)
: TransportPDU(len, std::move(src_port), std::move(dest_port))
{}

UDP::~UDP() = default;


std::string UDP::make_info() const {
    return "";
}

std::string UDP::name() const {
    return "UDP";
}

std::unique_ptr<TransportPDU> udp_functions::udp_parse(
    std::span<std::byte> raw_data,
    parse_context &context) {

    using namespace layer::transport;

    if (!PacketRead::valid_length(raw_data, context.offset, sizeof(udp_header))) {
        return nullptr;
    }

    const std::byte* start = raw_data.data() + context.offset;

    const auto udp_hdr = reinterpret_cast<const udp_header*> (start);

    size_t length = ntohs(udp_hdr->len);

    return std::make_unique<UDP>(
        length,
        std::to_string(ntohs(udp_hdr->src)),
        std::to_string(ntohs(udp_hdr->dest))
        );
}

ProtocolDetails udp_functions::udp_detailed_parse(
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

    return { full_protocol_name(), details };
}

