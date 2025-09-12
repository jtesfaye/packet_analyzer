//
// Created by jeremiah tesfaye on 9/1/25.
//

#ifndef LAYER3TYPES_H
#define LAYER3TYPES_H
#include <cstdint>

struct NetworkPDU : ProtocolDataUnit {

    NetworkPDU(size_t len, std::string src, std::string dest)
    : ProtocolDataUnit(len, std::move(src), std::move(dest))
    {}

    virtual std::string make_info() const = 0;
    virtual std::string name() const = 0;
    virtual ~NetworkPDU();

};

namespace layer::ip {

    namespace iana { //Values assigned by IANA

        constexpr u_int16_t IPV4 = 0x0800;
        constexpr u_int16_t IPV6 = 0x08DD;

        constexpr u_int8_t ICMP = 1;
        constexpr u_int8_t TCP = 6;
        constexpr u_int8_t UDP = 17;

    }




    constexpr u_int16_t UNSUPPORTED = 0x0000;

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

    };

}


#endif //LAYER3TYPES_H
