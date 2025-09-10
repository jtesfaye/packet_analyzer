//
// Created by jeremiah tesfaye on 7/27/25.
//

#ifndef ETHERNET_H
#define ETHERNET_H

#include <vector>
#include <format>
#include <packet/LayerWrappers.h>
#include <layer2_protocols/Layer2Types.h>

using namespace packet::frame;
using namespace packet;

struct Ethernet final : LinkPDU {

    Ethernet(size_t len, std::string src, std::string dest, u_int16_t ether_type);

    std::string make_info() const override;
    std::string name() const override;

    u_int16_t ether_type;

};

class Ethernet_functions {
public:

    static std::unique_ptr<Ethernet> ethernet_parse(const std::vector<std::byte>&, parse_context&);

};




#endif //ETHERNET_H