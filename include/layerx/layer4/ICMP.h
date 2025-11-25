//
// Created by jeremiah tesfaye on 9/16/25.
//

#ifndef ICMP_H
#define ICMP_H

#include <packet/PacketUtil.h>
#include <vector>


struct ICMP final : TransportPDU {

    ICMP(size_t len, u_int8_t type, u_int8_t code);
    ~ICMP() override = default;

    std::string make_info() const override;
    std::string_view name() const override;
    Address src() const override {return {};}
    Address dest() const override {return {};}
    ProtocolKeys type() const override;
    u_int8_t type_field;
    u_int8_t code;

    ProtocolKeys key = ProtocolKeys::ICMP;

};

namespace protocol::icmp {

    std::unique_ptr<TransportPDU> icmp_parse(
        std::span<std::byte> raw_data,
        packet::parse_context& context);

    void register_icmp();

    std::string type_code_to_string(u_int8_t type, u_int8_t code);

    inline constexpr std::string_view full_protocol_name = "Internet Control Message Protocol";
    inline constexpr std::string_view name = "ICMP";

    struct icmp_header {

        u_int8_t type;
        u_int8_t code;
        u_int16_t checksum;
        u_int32_t data;

    } __attribute__((packed));

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

#endif //ICMP_H
