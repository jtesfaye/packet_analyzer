//
// Created by jeremiah tesfaye on 9/17/25.
//

#ifndef LAYER2REGISTRY_H
#define LAYER2REGISTRY_H
#include <functional>
#include <vector>
#include <layerx/ParseFormat.h>
#include <layerx/layer2/Ethernet.h>

namespace registry::layer2 {

    static std::vector<std::pair<int, parse::function>> layer2_initial_parse_registry;
    static std::vector<std::pair<int, parse::detail_function>> layer2_detail_parse_registry;

    void register_self(int key, const parse::function& func) {
        layer2_initial_parse_registry.push_back(std::pair{key, func});
    }

    void register_self(int key, const parse::detail_function& func) {
        layer2_detail_parse_registry.push_back(std::pair{key, func});
    }

    inline void register_all_functions() {
        protocol::ethernet::register_ethernet();
    }
}

#endif //LAYER2REGISTRY_H
