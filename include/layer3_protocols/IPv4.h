//
// Created by jeremiah tesfaye on 8/31/25.
//

#ifndef IPV4_H
#define IPV4_H
#include <packet/LayerWrappers.h>
#include <packet/ProtocolTypes.h>
#include <vector>

using namespace packet;

class IPv4_functions {
public:

    static net_layer_ref IPv4_parse(const std::vector<std::byte>&, parse_context&);

private:

    static std::string ipv4_src_dest_format(const std::byte*);

};
#endif //IPV4_H
