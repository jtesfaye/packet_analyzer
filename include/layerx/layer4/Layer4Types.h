//
// Created by jeremiah tesfaye on 9/3/25.
//

#ifndef LAYER4TYPES_H
#define LAYER4TYPES_H

#include <layerx/ProtocolDataUnit.h>

struct TransportPDU : ProtocolDataUnit {

    TransportPDU(size_t len, std::string src, std::string dest)
    : ProtocolDataUnit(len, std::move(src), std::move(dest))
    {}

    ~TransportPDU() override = default;

};

namespace layer::transport {

    namespace tcp_flags {

        constexpr u_int8_t CWR = 0x80;
        constexpr u_int8_t ECE = 0x40;
        constexpr u_int8_t URG = 0x20;
        constexpr u_int8_t ACK = 0x10;
        constexpr u_int8_t PSH = 0x08;
        constexpr u_int8_t RST = 0x04;
        constexpr u_int8_t SYN = 0x02;
        constexpr u_int8_t FIN = 0x01;

    }

    struct tcp_header {

        u_int16_t src;
        u_int16_t dest;
        u_int32_t sequence;
        u_int32_t ack;
        u_int8_t offset;
        u_int8_t flags;
        u_int16_t window;
        u_int16_t chksum;
        u_int16_t urgent;

    };

    struct udp_header {

        u_int16_t src;
        u_int16_t dest;
        u_int16_t len;
        u_int16_t checksum;

    };

    struct icmp_header {

        u_int8_t type;
        u_int8_t code;
        u_int16_t checksum;
        u_int32_t data;

    };

    namespace icmp_types {

        constexpr u_int8_t ECHO_REPLY = 0;
        constexpr u_int8_t DEST_UNREACHABLE = 3;
        constexpr u_int8_t ECHO_REQUEST = 8;
        constexpr u_int8_t SOURCE_QUENCH = 4;
        constexpr u_int8_t REDIRECT = 5;
        constexpr u_int8_t TIME_EXCEEDED = 11;
        constexpr u_int8_t PARAMETER_PROBLEM = 12;
        constexpr u_int8_t TIMESTAMP_REQUEST = 13;
        constexpr u_int8_t TIMESTAMP_REPLY = 14;
        constexpr u_int8_t INFORMATION_REQUEST = 15;
        constexpr u_int8_t INFORMATION_REPLY = 16;
        constexpr u_int8_t ADDRESS_MASK_REQUEST = 17;
        constexpr u_int8_t ADDRESS_MASK_REPLY = 18;


    }
}


#endif //LAYER4TYPES_H
