//
// Created by jeremiah tesfaye on 11/20/25.
//

#include <layerx/layer3/Layer3Registry.h>
#include <layerx/layer3/IPv4.h>
#include <layerx/layer3/IPv6.h>

void registry::layer3::register_self(int key, const parse::function& func) {
    layer3_initial_parse_registry.push_back(std::pair{key, func});
}

void registry::layer3::register_self(int key, const parse::detail_function& func) {
    layer3_detail_parse_registry.push_back(std::pair{key, func});
}

void registry::layer3::register_all_functions() {

    using namespace protocol;
    ipv4::register_ipv4();
    ipv6::register_ipv6();
}