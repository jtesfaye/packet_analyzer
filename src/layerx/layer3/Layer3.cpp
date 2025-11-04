//
// Created by jeremiah tesfaye on 9/18/25.
//

#include <layerx/layer3/Layer3.h>
#include <layerx/layer3/IPv4.h>
#include <layerx/layer3/IPv6.h>

void Layer3::register_all_functions() {

    IPv6_functions::register_ipv6();
    IPv4_functions::register_ipv4();

}

const std::vector<std::pair<int, Layer::function>>& Layer3::get_first_parse_registry() {
    return Layer3Registry::get_registry();
}

const std::vector<std::pair<int, Layer::detail_function> > &Layer3::get_detail_parse_registry() {
    return Layer3Registry::get_detail_registry();
}


