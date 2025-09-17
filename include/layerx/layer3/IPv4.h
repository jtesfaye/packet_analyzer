//
// Created by jeremiah tesfaye on 8/31/25.
//

#ifndef IPV4_H
#define IPV4_H
#include <packet/PacketUtil.h>
#include <layerx/layer3/Layer3Types.h>
#include <vector>

using namespace layer::ip;

struct IPv4 final : NetworkPDU {

    IPv4(size_t len, std::string src, std::string dest, bool is_fragmented, u_int8_t protocol);
    ~IPv4() override;

    std::string make_info() const override;
    std::string name() const override;

    u_int8_t protocol;
    bool is_fragmented;

};

class IPv4_functions {
public:

    static std::unique_ptr<NetworkPDU> ipv4_parse(const std::vector<std::byte>&, packet::parse_context&);

};
#endif //IPV4_H
