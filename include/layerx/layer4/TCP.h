//
// Created by jeremiah tesfaye on 9/3/25.
//

#ifndef TCP_H
#define TCP_H

#include <packet/PacketUtil.h>
#include <vector>

using namespace packet;

struct TCP final : TransportPDU {

    TCP(size_t len, u_int16_t src_port, u_int16_t dest_port, u_int8_t flags);
    ~TCP() override;

    std::string make_info() const override;
    std::string_view name() const override;
    std::string address_to_string(const Address& addr) const override;
    Address src() const override;
    Address dest() const override;
    ProtocolKeys type() const override;

    Address src_address;
    Address dest_address;
    u_int8_t flags;
    ProtocolKeys key = ProtocolKeys::TCP;

};

namespace protocol::tcp {

    std::unique_ptr<TransportPDU> tcp_parse(
        std::span<std::byte> raw_data,
        parse_context& context);

    ProtocolDetails tcp_detailed_parse(
       std::span<std::byte> raw_data,
       parse_context& context);

    void register_tcp();

    std::string tcp_flags_to_string(u_int8_t flags);

    inline constexpr std::string_view name = "TCP";
    inline constexpr std::string_view full_protocol_name = "Transmission Control Protocol";
    inline constexpr size_t addr_len = 2;

    namespace flags {

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

    } __attribute__((packed));

}



#endif //TCP_H
