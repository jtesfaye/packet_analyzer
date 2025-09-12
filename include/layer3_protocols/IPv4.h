//
// Created by jeremiah tesfaye on 8/31/25.
//

#ifndef IPV4_H
#define IPV4_H
#include <packet/LayerWrappers.h>
#include <packet/ProtocolTypes.h>
#include <vector>

using namespace layer::ip;

struct IPv4 : NetworkPDU {

    IPv4(size_t len, std::string src, std::string dest, u_int16_t frag_field, u_int8_t protocol);

    u_int8_t protocol;
    u_int16_t flags;

    std::string make_info() const override;
    std::string name() const override;


};

class IPv4_functions {
public:

    static std::unique_ptr<IPv4> IPv4_parse(const std::vector<std::byte>&, packet::parse_context&);

};
#endif //IPV4_H
