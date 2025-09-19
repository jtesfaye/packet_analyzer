//
// Created by jeremiah tesfaye on 9/1/25.
//

#ifndef LAYER3TYPES_H
#define LAYER3TYPES_H

#include <cstdint>
#include <layerx/ProtocolDataUnit.h>

struct NetworkPDU : ProtocolDataUnit {

    NetworkPDU(size_t len, std::string src, std::string dest)
    : ProtocolDataUnit(len, std::move(src), std::move(dest))
    {}

    ~NetworkPDU() override = default;

};

namespace layer::ip {

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

    struct ipv6_header {

        uint32_t ver_tc_fl;    // version, traffic class, flow label
        uint16_t payload_length;
        uint8_t  next_header;
        uint8_t  hop_limit;
        uint8_t  src_addr[16];
        uint8_t  dst_addr[16];
    };

}


#endif //LAYER3TYPES_H
