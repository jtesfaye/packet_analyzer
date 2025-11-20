//
// Created by jeremiah tesfaye on 9/3/25.
//

#include <layerx/layer4/TCP.h>
#include <layerx/layer4/Layer4Registry.h>
#include <format>

TCP::TCP(const size_t len, u_int16_t src_port, u_int16_t dest_port, const u_int8_t flags)
: TransportPDU(len)
, flags(flags) {

    using namespace protocol::tcp;
    std::memcpy(&src_address.bytes, &src_port, addr_len);
    std::memcpy(&dest_address.bytes, &dest_port, addr_len);
    src_address.size = addr_len;
    dest_address.size = addr_len;
}

TCP::~TCP() = default;

std::string TCP::make_info() const {

    u_int16_t src_p;
    u_int16_t dest_p;

    std::memcpy(&src_p, &src_address.bytes, protocol::tcp::addr_len);
    std::memcpy(&dest_p, &dest_address.bytes, protocol::tcp::addr_len);

    std::string info = std::format("{} -> {} ", src_p, dest_p);

    info += protocol::tcp::tcp_flags_to_string(flags);

    return info;
}

std::string_view TCP::name() const {
    return protocol::tcp::name;
}

std::string TCP::address_to_string(const Address &addr) const {

    u_int16_t target{};
    std::memcpy(&target, addr.bytes.data(), sizeof(target));
    return std::to_string(target);
}

Address TCP::src() const {
    return src_address;
}

Address TCP::dest() const {
    return dest_address;
}

void protocol::tcp::register_tcp() {

    registry::layer4::register_self(iana_number, tcp_parse);
    registry::layer4::register_self(iana_number, tcp_detailed_parse);
}

std::unique_ptr<TransportPDU> protocol::tcp::tcp_parse(
    std::span<std::byte> raw_data,
    parse_context &context) {

    if (!valid_length(raw_data, context.offset, sizeof(tcp_header))) {
        return nullptr;
    }

    const std::byte* start = raw_data.data() + context.offset;

    const auto tcp_hdr = reinterpret_cast<const tcp_header*> (start);

    size_t length = (tcp_hdr->offset >> 4) * 4;

    return std::make_unique<TCP>(
        length,
        ntohs(tcp_hdr->src),
        ntohs(tcp_hdr->dest),
        tcp_hdr->flags);
}

ProtocolDetails protocol::tcp::tcp_detailed_parse(
    std::span<std::byte> raw_data,
    parse_context& context) {

    const auto* hdr = reinterpret_cast<const tcp_header*>(raw_data.data() + context.offset);

    std::vector<std::string> details;
    details.reserve(10);

    details.emplace_back(std::format("Source port: {}", ntohs(hdr->src)));
    details.emplace_back(std::format("Destination port: {}", ntohs(hdr->dest)));
    details.emplace_back(std::format("Sequence number (raw): {}", ntohl(hdr->sequence)));
    details.emplace_back(std::format("Acknowledgement Number (raw) {}: ", ntohl(hdr->ack)));
    details.emplace_back(std::format("Header length: {} ({})", (hdr->offset >> 4) * 4, hdr->offset >> 4));
    details.emplace_back(std::format("Flags: {}", tcp_flags_to_string(hdr->flags)));
    details.emplace_back(std::format("Window: {}", ntohs(hdr->window)));
    details.emplace_back(std::format("Checksum: {}", ntohs(hdr->chksum)));
    details.emplace_back(std::format("Urgent pointer: {}", ntohs(hdr->urgent)));

    return {full_protocol_name ,details};
}

std::string protocol::tcp::tcp_flags_to_string(u_int8_t flags) {

    static constexpr std::array<std::pair<uint8_t, const char*>, 8> table{{
        {flags::SYN, "SYN"},
        {flags::ACK, "ACK"},
        {flags::FIN, "FIN"},
        {flags::RST, "RST"},
        {flags::PSH, "PSH"},
        {flags::URG, "URG"},
        {flags::ECE, "ECE"},
        {flags::CWR, "CWR"}
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

    if (flags & flags::SYN && flags & flags::FIN) {

        s += " (invalid: SYN+FIN)";

    } else if (flags & flags::FIN && flags & flags::PSH && flags & flags::URG) {

        s += " (christmas)";

    } else if (flags == 0xFF) {

        s += " (all-flags)";

    }

    return s;
}


