//
// Created by jeremiah tesfaye on 9/18/25.
//

#ifndef LAYER3REGISTRY_H
#define LAYER3REGISTRY_H

#include <functional>
#include <vector>
#include <iostream>
#include <layerx/layer3/IPv4.h>
#include <layerx/layer3/IPv6.h>
#include <layerx/ParseFormat.h>

namespace registry::layer3 {

    static std::vector<std::pair<int, parse::function>> layer3_initial_parse_registry;
    static std::vector<std::pair<int, parse::detail_function>> layer3_detail_parse_registry;

    void register_self(int key, const parse::function& func) {
        layer3_initial_parse_registry.push_back(std::pair{key, func});
    }

    void register_self(int key, const parse::detail_function& func) {
        layer3_detail_parse_registry.push_back(std::pair{key, func});
    }

    inline void register_all_functions() {

        using namespace protocol;
        ipv4::register_ipv4();
        ipv6::register_ipv6();
    }

}



#endif //LAYER3REGISTRY_H
