//
// Created by jeremiah tesfaye on 10/2/25.
//

#include <layerx/layer4/UDP.h>
#include <format>
#include <iostream>
#include <util/PacketRead.h>
#include <layerx/iana_numbers.h>

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

Layer4Registry& udp_functions::get_udp_registry() {
    static Layer4Registry udp_reg(layer::iana::UDP, udp_parse);
    return udp_reg;
}

std::unique_ptr<TransportPDU> udp_functions::udp_parse(
    const std::vector<std::byte> &raw_data,
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
