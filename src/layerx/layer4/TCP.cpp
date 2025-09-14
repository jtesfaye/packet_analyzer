//
// Created by jeremiah tesfaye on 9/3/25.
//

#include <layerx/layer4/TCP.h>
#include <format>
#include <utility>

TCP::TCP(size_t len, std::string src, std::string dest, u_int8_t flags)
: TransportPDU(len, std::move(src), std::move(dest))
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


std::unique_ptr<TransportPDU> tcp_functions::tcp_parse(
    const std::vector<std::byte> &raw_data,
    parse_context &context) {

    return {};

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

