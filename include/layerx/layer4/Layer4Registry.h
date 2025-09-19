//
// Created by jeremiah tesfaye on 9/18/25.
//

#ifndef LAYER4REGISTRY_H
#define LAYER4REGISTRY_H

#include <functional>
#include <vector>
#include <packet/PacketUtil.h>

using parse_function = std::function<std::unique_ptr<TransportPDU>(
    const std::vector<std::byte>&,
    packet::parse_context&)>;

struct Layer4Registry {
    static std::vector<std::pair<const int, parse_function>>& get_registry() {
        static std::vector<std::pair<const int, parse_function>> registry;
        return registry;
    }

    Layer4Registry(int key, parse_function func) {
        get_registry().push_back(std::pair{key, func});
    }

};

#endif //LAYER4REGISTRY_H
