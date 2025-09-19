//
// Created by jeremiah tesfaye on 9/16/25.
//

#ifndef ICMP_H
#define ICMP_H

#include <packet/PacketUtil.h>
#include <layerx/layer4/Layer4Types.h>
#include <vector>
#include <layerx/layer4/Layer4Registry.h>

/**
 * Technically, ICMP is a layer 3 protocol, since it can be found after ip protocols, I placed it with
 * the layer 4 protocols.
 */
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

    static Layer4Registry& get_icmp_registry();

    static std::string type_code_to_string(u_int8_t type, u_int8_t code);

};

#endif //ICMP_H
