//
// Created by jeremiah tesfaye on 9/18/25.
//

#ifndef IPV6_H
#define IPV6_H

#include <packet/PacketUtil.h>
#include <vector>

struct IPv6 final : NetworkPDU {

    IPv6(size_t len, const u_int8_t *src, const u_int8_t *dest, u_int8_t protocol);
    ~IPv6() override = default;

    std::string make_info() const override;
    std::string_view name() const override;
    std::string address_to_string(const Address& addr) const override;
    Address src() const override;
    Address dest() const override;
    ProtocolKeys type() const override;

    u_int8_t protocol;
    Address src_address;
    Address dest_address;

    ProtocolKeys key = ProtocolKeys::IPv6;

};

namespace protocol::ipv6 {

    using namespace packet;

    std::unique_ptr<NetworkPDU> ipv6_parse(std::span<std::byte>, parse_context&);

    ProtocolDetails ipv6_detailed_parse(std::span<std::byte>, parse_context&);

    void register_ipv6();

    inline constexpr std::string_view full_protocol_name = "Internet Protocol Version 6";
    inline constexpr std::string_view name = "UDP";
    inline constexpr size_t addr_len = 16;

    struct ipv6_header {
        uint32_t ver_tc_fl;    // version, traffic class, flow label
        uint16_t payload_length;
        uint8_t  next_header;
        uint8_t  hop_limit;
        uint8_t  src_addr[16];
        uint8_t  dst_addr[16];
    } __attribute__((packed));

    inline uint8_t ipv6_version(const ipv6_header* hdr) {
        return (ntohl(hdr->ver_tc_fl) >> 28) & 0xF;
    }
    inline uint8_t ipv6_traffic_class(const ipv6_header* hdr) {
        return (ntohl(hdr->ver_tc_fl) >> 20) & 0xFF;
    }
    inline uint32_t ipv6_flow_label(const ipv6_header* hdr) {
        return ntohl(hdr->ver_tc_fl) & 0xFFFFF;
    }


}

#endif //IPV6_H
