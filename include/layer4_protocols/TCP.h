//
// Created by jeremiah tesfaye on 9/3/25.
//

#ifndef TCP_H
#define TCP_H

#include <packet/LayerWrappers.h>
#include <packet/ProtocolTypes.h>
#include <vector>

using namespace packet::transport;
using namespace  packet;

class tcp_functions {
public:

    static transport_layer_ref tcp_parse(const std::vector<std::byte>& raw_data, parse_context& context);

};

#endif //TCP_H
