//
// Created by jeremiah tesfaye on 10/2/25.
//

#ifndef UDP_H
#define UDP_H

#include <packet/PacketUtil.h>
#include <layerx/ProtocolDataUnit.h>
#include <vector>

using namespace packet;

struct UDP : TransportPDU {

    UDP(size_t len, u_int16_t src_port, u_int16_t dest_port);
    ~UDP() override;

    std::string make_info() const override;
    std::string_view name() const override;
    std::string address_to_string(const Address& addr) const override;
    Address src() const override;
    Address dest() const override;
    ProtocolKeys type() const override;

    Address src_address;
    Address dest_address;
    ProtocolKeys key = ProtocolKeys::UDP;

};

namespace protocol::udp {

    std::unique_ptr<TransportPDU> udp_parse(
        std::span<std::byte> raw_data,
        parse_context& context);

    ProtocolDetails udp_detailed_parse(
        std::span<std::byte> raw_data,
        parse_context& context);

    void register_udp();

    inline constexpr std::string_view full_protocol_name = "User Datagram Protocol";
    inline constexpr std::string_view name = "UDP";
    inline constexpr size_t addr_len = 2;

    struct udp_header {

        u_int16_t src;
        u_int16_t dest;
        u_int16_t len;
        u_int16_t checksum;

    } __attribute__((packed));
}


#endif //UDP_H
