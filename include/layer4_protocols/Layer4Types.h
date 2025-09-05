//
// Created by jeremiah tesfaye on 9/3/25.
//

#ifndef LAYER4TYPES_H
#define LAYER4TYPES_H

namespace packet::transport {

    constexpr u_int8_t TCP_IANA = 6; //number assigned to TCP by IANA
    constexpr u_int8_t TCP_OFFSET = 0xf0;
    constexpr u_int8_t TCP_CWR = 0x80;
    constexpr u_int8_t TCP_ECE = 0x40;
    constexpr u_int8_t TCP_URG = 0x20;
    constexpr u_int8_t TCP_ACK = 0x10;
    constexpr u_int8_t TCP_PSH = 0x08;
    constexpr u_int8_t TCP_RST = 0x04;
    constexpr u_int8_t TCP_SYN = 0x02;
    constexpr u_int8_t TCP_FIN = 0x01;


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
