//
// Created by jeremiah tesfaye on 7/27/25.
//

#ifndef ETHERNET_H
#define ETHERNET_H

#include <vector>
#include <format>
#include <packet/PacketUtil.h>
using namespace packet;

struct Ethernet final : LinkPDU {

    Ethernet(size_t len, u_int16_t src, u_int16_t dest, u_int16_t ether_type);
    ~Ethernet() override;

    std::string make_info() const override;
    std::string_view name() const override;

    u_int16_t ether_type;

};

namespace protocol::ethernet {

    std::unique_ptr<LinkPDU> ethernet_parse(std::span<std::byte>, parse_context&);
    ProtocolDetails ethernet_detailed_parse(std::span<std::byte>, parse_context&);
    void register_ethernet();

    inline constexpr std::string_view full_protocol_name = "802.3 Ethernet";
    inline constexpr std::string_view name = "Ethernet";
    inline constexpr u_int8_t IEEE_802_3 = 0x05DC;

    struct ethernet_hdr {

        u_int8_t dest_addr[6];
        u_int8_t src_addr[6];
        u_int16_t ether_type;

    };

    struct ether_802_1_Q_hdr {

        u_int8_t dest_addr[6];
        u_int8_t src_addr[6];
        u_int16_t tpid;
        u_int16_t tci;
        u_int16_t ether_type;

    };

    struct ether_802_1_ad_hdr {

        u_int8_t dest_addr[6];
        u_int8_t src_addr[6];
        u_int16_t tpid;
        u_int16_t tci;
        u_int16_t tpid_2;
        u_int16_t tci_2;
        u_int16_t ether_type;

    };

    struct _802_2_hdr {

        u_int8_t DSAP_addr;
        u_int8_t SSAP_addr;
        u_int8_t control;

    };

    struct snap_extension_hdr {

        u_int8_t oui[3]; //organizationally unique identifier, not 'yes' in french
        u_int16_t protocol_id;

    };

}


#endif //ETHERNET_H