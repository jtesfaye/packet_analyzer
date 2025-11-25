//
// Created by jeremiah tesfaye on 10/2/25.
//

#include <layerx/layer4/UDP.h>
#include <format>
#include <layerx/layer4/Layer4Registry.h>

UDP::UDP(const size_t len, const u_int16_t src_port, const u_int16_t dest_port)
: TransportPDU(len) {

    using namespace protocol::udp;
    std::memcpy(&src_address.bytes, &src_port, addr_len);
    std::memcpy(&dest_address.bytes, &dest_port, addr_len);
    src_address.size = addr_len;
    dest_address.size = addr_len;
}

UDP::~UDP() = default;


std::string UDP::make_info() const {
    return "";
}

std::string_view UDP::name() const {
    return udp::name;
}

std::string UDP::address_to_string(const Address &addr) const {
    u_int16_t target{};
    std::memcpy(&target, addr.bytes.data(), sizeof(target));
    return std::to_string(target);
}

Address UDP::src() const {
    return src_address;
}

Address UDP::dest() const {
    return dest_address;
}

ProtocolKeys UDP::type() const {
    return key;
}


void udp::register_udp() {
    registry::layer4::register_self(static_cast<int>(ProtocolKeys::UDP), udp_parse);
    registry::layer4::register_self(static_cast<int> (ProtocolKeys::UDP), udp_detailed_parse);
}

std::unique_ptr<TransportPDU> udp::udp_parse(
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

ProtocolDetails udp::udp_detailed_parse(
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

