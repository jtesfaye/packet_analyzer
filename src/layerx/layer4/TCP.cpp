//
// Created by jeremiah tesfaye on 9/3/25.
//

#include <layerx/layer4/TCP.h>
#include <format>
#include <iostream>
#include <util/PacketRead.h>
#include <layerx/iana_numbers.h>

TCP::TCP(const size_t len, std::string src_port, std::string dest_port, const u_int8_t flags)
: TransportPDU(len, std::move(src_port), std::move(dest_port))
, flags(flags)
{}

TCP::~TCP() = default;


std::string TCP::make_info() const {

    std::string info = std::format("{} -> {} [", src, dest);

    info += tcp_functions::tcp_flags_to_string(flags);

    return info;

}

std::string TCP::name() const {
    return "TCP";
}

Layer4Registry& tcp_functions::get_tcp_registry() {
    static Layer4Registry tcp_reg(layer::iana::TCP, tcp_parse);
    return tcp_reg;
}


std::unique_ptr<TransportPDU> tcp_functions::tcp_parse(
    const std::vector<std::byte> &raw_data,
    parse_context &context) {

    using namespace layer::transport;

    if (!PacketRead::valid_length(raw_data, context.offset, sizeof(tcp_header))) {
        return nullptr;
    }

    const std::byte* start = raw_data.data() + context.offset;

    const auto tcp_hdr = reinterpret_cast<const tcp_header*> (start);

    size_t length = (tcp_hdr->offset >> 4) * 4;

    return std::make_unique<TCP>(
        length,
        std::to_string(ntohs(tcp_hdr->src)),
        std::to_string(ntohs(tcp_hdr->dest)),
        tcp_hdr->flags);

}

std::string tcp_functions::tcp_flags_to_string(u_int8_t flags) {

    static constexpr std::array<std::pair<uint8_t, const char*>, 8> table{{
        {tcp_flags::SYN, "SYN"},
        {tcp_flags::ACK, "ACK"},
        {tcp_flags::FIN, "FIN"},
        {tcp_flags::RST, "RST"},
        {tcp_flags::PSH, "PSH"},
        {tcp_flags::URG, "URG"},
        {tcp_flags::ECE, "ECE"},
        {tcp_flags::CWR, "CWR"}
    }};

    std::string s;
    std::vector<const char*> parts;
    for (auto &p : table) {
        if (flags & p.first) parts.push_back(p.second);
    }

    if (parts.empty()) {

        s = "[]";

    } else {

        s = "[";

        for (size_t i = 0; i < parts.size(); ++i) {

            if (i) s += ", ";

            s += parts[i];

        }

        s += "]";

    }

    if (flags & tcp_flags::SYN && flags & tcp_flags::FIN) {

        s += " (invalid: SYN+FIN)";

    } else if (flags & tcp_flags::FIN && flags & tcp_flags::PSH && flags & tcp_flags::URG) {

        s += " (christmas)";

    } else if (flags == 0xFF) {

        s += " (all-flags)";

    }

    return s;
}


