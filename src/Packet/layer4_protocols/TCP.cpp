//
// Created by jeremiah tesfaye on 9/3/25.
//

#include <layer4_protocols/TCP.h>

transport_layer_ref
tcp_functions::tcp_parse(const std::vector<std::byte> &raw_data, parse_context &context) {

    return transport_layer_ref{static_cast<tcp_header*>(nullptr)};

}
