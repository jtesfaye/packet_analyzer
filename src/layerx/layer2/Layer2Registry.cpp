//
// Created by jeremiah tesfaye on 11/20/25.
//

#include <iostream>
#include <layerx/layer2/Layer2Registry.h>
#include <layerx/layer2/Ethernet.h>

void registry::layer2::register_self(int key, const parse::function& func) {
    layer2_initial_parse_registry.push_back(std::pair{key, func});
    std::cout << layer2_initial_parse_registry.size() << "\n";
}

void registry::layer2::register_self(int key, const parse::detail_function& func) {
    layer2_detail_parse_registry.push_back(std::pair{key, func});
}

void registry::layer2::register_all_functions() {
    protocol::ethernet::register_ethernet();
}

