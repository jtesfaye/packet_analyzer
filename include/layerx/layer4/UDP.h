//
// Created by jeremiah tesfaye on 10/2/25.
//

#ifndef UDP_H
#define UDP_H

#include <packet/PacketUtil.h>
#include <layerx/layer4/Layer4.h>
#include <vector>

using namespace packet;

struct UDP : TransportPDU {

    UDP(size_t len, u_int16_t src_port, u_int16_t dest_port);
    ~UDP() override;

    std::string make_info() const override;
    std::string_view name() const override;

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

    struct udp_header {

        u_int16_t src;
        u_int16_t dest;
        u_int16_t len;
        u_int16_t checksum;

    };
}


#endif //UDP_H
