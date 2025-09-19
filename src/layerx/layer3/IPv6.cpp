//
// Created by jeremiah tesfaye on 9/18/25.
//

#include <layerx/layer3/IPv6.h>
#include <layerx/iana_numbers.h>
#include <utility>

IPv6::IPv6(size_t len, std::string src, std::string dest, u_int8_t protocol)
: NetworkPDU(len, std::move(src), std::move(dest))
, protocol(protocol) {

}

std::string IPv6::make_info() const {

    using namespace layer;

    std::string info;

    switch (protocol) {

        case iana::ICMP:
            info += "ICMP ";
        break;

        case iana::TCP:
            info += "TCP ";
        break;

        case iana::UDP:
            info += "UDP ";
        break;

        default:
    }

    return info;

}

std::string IPv6::name() const {
    return "IPv6";
}

Layer3Registry &IPv6_functions::get_ipv6_registry() {
    static Layer3Registry ipv6_reg(layer::iana::IPV6, ipv6_parse);
    return ipv6_reg;

}

std::unique_ptr<NetworkPDU> IPv6_functions::ipv6_parse(
    const std::vector<std::byte>&,
    packet::parse_context&) {

    return {};
}





