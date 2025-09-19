//
// Created by jeremiah tesfaye on 9/18/25.
//

#ifndef LAYER3REGISTRY_H
#define LAYER3REGISTRY_H

#include <functional>
#include <vector>
#include <packet/PacketUtil.h>

using parse_function = std::function<std::unique_ptr<NetworkPDU>(
    const std::vector<std::byte>&,
    packet::parse_context&)>;

struct Layer3Registry {
    static std::vector<std::pair<const int, parse_function>>& get_registry() {
        static std::vector<std::pair<const int, parse_function>> registry;
        return registry;
    }

    Layer3Registry(int key, parse_function func) {
        get_registry().push_back(std::pair{key, func});
    }

};

#endif //LAYER3REGISTRY_H
