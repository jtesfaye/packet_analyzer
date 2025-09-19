//
// Created by jeremiah tesfaye on 9/17/25.
//

#ifndef LAYER2REGISTRY_H
#define LAYER2REGISTRY_H
#include <functional>
#include <vector>
#include <packet/PacketUtil.h>

using parse_function = std::function<std::unique_ptr<LinkPDU>(
    const std::vector<std::byte>&,
    packet::parse_context&)>;

struct Layer2Registry {
    static std::vector<std::pair<const int, parse_function>>& get_registry() {
        static std::vector<std::pair<const int, parse_function>> registry;
        return registry;
    }

    Layer2Registry(int key, parse_function func) {
        get_registry().push_back(std::pair{key, func});
    }

};
#endif //LAYER2REGISTRY_H
