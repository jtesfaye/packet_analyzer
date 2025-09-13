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

    virtual ~TransportPDU();

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

    struct upd_header {

    };
}


#endif //LAYER4TYPES_H
