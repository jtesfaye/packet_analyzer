//
// Created by jeremiah tesfaye on 9/3/25.
//

#ifndef TCP_H
#define TCP_H

#include <packet/PacketUtil.h>
#include <layerx/layer4/Layer4Types.h>
#include <vector>

using namespace layer::transport;
using namespace  packet;

struct TCP : TransportPDU {

    TCP(size_t len, std::string src, std::string dest, u_int8_t flags);

    std::string make_info() const override;
    std::string name() const override;

    u_int8_t flags;

};

class tcp_functions {
public:

    static std::unique_ptr<TransportPDU> tcp_parse(const std::vector<std::byte>& raw_data, parse_context& context);

    static std::string tcp_flags_to_string(u_int8_t flags);

};



#endif //TCP_H
