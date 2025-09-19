//
// Created by jeremiah tesfaye on 9/18/25.
//

#ifndef IPV6_H
#define IPV6_H

#include <packet/PacketUtil.h>
#include <layerx/layer3/Layer3Types.h>
#include <layerx/layer3/Layer3Registry.h>
#include <vector>

using namespace layer::ip;

struct IPv6 final : NetworkPDU {

    IPv6(size_t len, std::string src, std::string dest, u_int8_t protocol);
    ~IPv6() override = default;

    std::string make_info() const override;
    std::string name() const override;

    u_int8_t protocol;

};

class IPv6_functions {
public:
    static std::unique_ptr<NetworkPDU> ipv6_parse(const std::vector<std::byte>&, packet::parse_context&);
    static Layer3Registry& get_ipv6_registry();

private:

    static uint8_t ipv6_version(const ipv6_header* hdr) {
        return (ntohl(hdr->ver_tc_fl) >> 28) & 0xF;
    }
    static uint8_t ipv6_traffic_class(const ipv6_header* hdr) {
        return (ntohl(hdr->ver_tc_fl) >> 20) & 0xFF;
    }
    static uint32_t ipv6_flow_label(const ipv6_header* hdr) {
        return ntohl(hdr->ver_tc_fl) & 0xFFFFF;
    }
};

#endif //IPV6_H
