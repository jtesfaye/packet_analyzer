//
// Created by jeremiah tesfaye on 9/18/25.
//

#include <layerx/layer4/Layer4.h>
#include <layerx/layer4/TCP.h>
#include <layerx/layer4/ICMP.h>

std::unique_ptr<TransportPDU> Layer4::unsupported_layer(
    const std::vector<std::byte>&,
    packet::parse_context&) {
    return nullptr;
}

const std::vector<std::pair<const int, Layer4::function> > Layer4::get_all_functions() {

    (void)tcp_functions::get_tcp_registry();
    (void)icmp_functions::get_icmp_registry();
    Layer4Registry(-1, unsupported_layer);

    return Layer4Registry::get_registry();
}
