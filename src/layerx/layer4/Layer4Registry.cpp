//
// Created by jeremiah tesfaye on 11/20/25.
//

#include <layerx/layer4/Layer4Registry.h>
#include <layerx/layer4/TCP.h>
#include <layerx/layer4/UDP.h>
#include <layerx/layer4/ICMP.h>

void registry::layer4::register_self(int key, const parse::function& func) {
    layer4_initial_parse_registry.push_back(std::pair{key, func});
}

void registry::layer4::register_self(int key, const parse::detail_function& func) {
    layer4_detail_parse_registry.push_back(std::pair{key, func});
}

void registry::layer4::register_all_functions() {
    using namespace protocol;
    tcp::register_tcp();
    udp::register_udp();
    icmp::register_icmp();
}