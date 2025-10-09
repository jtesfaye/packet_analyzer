//
// Created by jeremiah tesfaye on 9/12/25.
//

#ifndef IANA_NUMBERS_H
#define IANA_NUMBERS_H

#include <cstdint>

namespace layer::iana {

    constexpr u_int16_t IPV4 = 0x0800;
    constexpr u_int16_t IPV6 = 0x86DD;

    constexpr u_int8_t ICMP = 1;
    constexpr u_int8_t TCP = 6;
    constexpr u_int8_t UDP = 17;

    constexpr u_int16_t UNSUPPORTED = 0x0000;

}

#endif //IANA_NUMBERS_H
