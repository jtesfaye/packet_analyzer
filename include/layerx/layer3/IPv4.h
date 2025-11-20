//
// Created by jeremiah tesfaye on 8/31/25.
//

#ifndef IPV4_H
#define IPV4_H

#include <packet/PacketUtil.h>
#include <vector>
#include <layerx/layer3/Layer3.h>


struct IPv4 final : NetworkPDU {

    IPv4(size_t len, u_int16_t src, u_int16_t dest, bool is_fragmented, u_int8_t protocol);
    ~IPv4() override;

    std::string make_info() const override;
    std::string_view name() const override;

    u_int8_t protocol;
    bool is_fragmented;

};

namespace protocol::ipv4 {

    using namespace packet;

    std::unique_ptr<NetworkPDU> ipv4_parse(std::span<std::byte>, parse_context&);

    ProtocolDetails ipv4_detailed_parse(std::span<std::byte>, parse_context&);

    void register_ipv4();

    inline constexpr std::string_view full_protocol_name = "Internet Protocol Version 4";
    inline constexpr std::string_view name = "IPv4";

    constexpr uint16_t IP_RF = 0x8000;
    constexpr uint16_t IP_DF = 0x4000;
    constexpr uint16_t IP_MF = 0x2000;
    constexpr uint16_t IP_OF = 0x1fff;

    struct ipv4_header {

        u_int8_t version_ihl; //4 bits for version, 4 for ihl
        u_int8_t dscp_ecn; //6 bits for dscp, 2 bits for ecn
        u_int16_t length;
        u_int16_t id;
        u_int16_t flags_foffset; //3 bits for flags, 13 for fragment offset
        u_int8_t ttl;
        u_int8_t protocol;
        u_int16_t chksum;
        u_int32_t src_addr;
        u_int32_t dest_adr;

    };
}

#endif //IPV4_H
