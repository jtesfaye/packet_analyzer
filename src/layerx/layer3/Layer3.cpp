//
// Created by jeremiah tesfaye on 9/18/25.
//

#include <layerx/layer3/Layer3.h>
#include <layerx/layer3/IPv4.h>
#include <layerx/layer3/IPv6.h>

std::unique_ptr<NetworkPDU> Layer3::unsupported_type(
    const std::vector<std::byte>&,
    packet::parse_context&) {
    return nullptr;
}

void Layer3::register_all_functions() {

    (void)IPv4_functions::get_ipv4_registry();
    (void)IPv6_functions::get_ipv6_registry();
    Layer3Registry(-1, unsupported_type);

}

const std::vector<std::pair<int, Layer::function>>& Layer3::get_first_parse_registry() {
    return Layer3Registry::get_registry();
}

const std::vector<std::pair<int, Layer::detail_function> > &Layer3::get_detail_parse_registry() {
    return Layer3Registry::get_detail_registry();
}


