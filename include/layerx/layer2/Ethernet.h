//
// Created by jeremiah tesfaye on 7/27/25.
//

#ifndef ETHERNET_H
#define ETHERNET_H

#include <vector>
#include <format>
#include <packet/PacketUtil.h>
#include <layerx/layer2/Layer2Types.h>
#include <layerx/layer2/Layer2Registry.h>

struct Ethernet final : LinkPDU {

    Ethernet(size_t len, std::string src, std::string dest, u_int16_t ether_type);
    ~Ethernet() override;

    std::string make_info() const override;
    std::string name() const override;

    u_int16_t ether_type;

};

class ethernet_functions {
public:

    static std::unique_ptr<LinkPDU> ethernet_parse(const std::vector<std::byte>&, parse_context&);
    static ProtocolDetails ethernet_detailed_parse(const std::vector<std::byte>&, parse_context&);
    static void register_ethernet();

private:
    static std::string full_protocol_name() {
        return "802.3 Ethernet";
    }

};

#endif //ETHERNET_H