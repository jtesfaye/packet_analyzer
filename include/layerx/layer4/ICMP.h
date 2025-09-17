//
// Created by jeremiah tesfaye on 9/16/25.
//

#ifndef ICMP_H
#define ICMP_H

#include <packet/PacketUtil.h>
#include <layerx/layer4/Layer4Types.h>
#include <vector>

struct ICMP final : TransportPDU {

    ICMP(size_t len, u_int8_t type, u_int8_t code);
    ~ICMP() override = default;

    std::string make_info() const override;
    std::string name() const override;

    u_int8_t type;
    u_int8_t code;

};

class icmp_functions {
public:

    static std::unique_ptr<TransportPDU> icmp_parse(
        const std::vector<std::byte>& raw_data,
        packet::parse_context& context);

    static std::string type_code_to_string(u_int8_t type, u_int8_t code);

};

#endif //ICMP_H
