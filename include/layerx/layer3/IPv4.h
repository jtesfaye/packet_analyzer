//
// Created by jeremiah tesfaye on 8/31/25.
//

#ifndef IPV4_H
#define IPV4_H

#include <packet/PacketUtil.h>
#include <layerx/layer3/Layer3Types.h>
#include <layerx/layer3/Layer3Registry.h>
#include <vector>

using namespace layer::ip;

struct IPv4 final : NetworkPDU {

    IPv4(size_t len, std::string src, std::string dest, bool is_fragmented, u_int8_t protocol);
    ~IPv4() override;

    std::string make_info() const override;
    std::string name() const override;

    u_int8_t protocol;
    bool is_fragmented;

};

class IPv4_functions {
public:

    static std::unique_ptr<NetworkPDU> ipv4_parse(std::span<std::byte>, parse_context&);

    static ProtocolDetails ipv4_detailed_parse(std::span<std::byte>, parse_context&);

    static void register_ipv4();

    static std::string full_protocol_name() {
        return "Internet protocol version 4";
    }

};
#endif //IPV4_H
