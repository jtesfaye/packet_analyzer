//
// Created by jeremiah tesfaye on 9/18/25.
//

#ifndef LAYER4REGISTRY_H
#define LAYER4REGISTRY_H

#include <functional>
#include <vector>
#include <layerx/ParseFormat.h>
#include <layerx/layer4/TCP.h>
#include <layerx/layer4/UDP.h>
#include <layerx/layer4/ICMP.h>

namespace registry::layer4 {

    static std::vector<std::pair<int, parse::function>> layer4_initial_parse_registry;
    static std::vector<std::pair<int, parse::detail_function>> layer4_detail_parse_registry;

    inline void register_self(int key, const parse::function& func) {
        layer4_initial_parse_registry.push_back(std::pair{key, func});
    }

    inline void register_self(int key, const parse::detail_function& func) {
        layer4_detail_parse_registry.push_back(std::pair{key, func});
    }

    inline void register_all_functions() {
        using namespace protocol;
        tcp::register_tcp();
        udp::register_udp();
        icmp::register_icmp();
    }

}

#endif //LAYER4REGISTRY_H
