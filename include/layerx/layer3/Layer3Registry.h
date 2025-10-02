//
// Created by jeremiah tesfaye on 9/18/25.
//

#ifndef LAYER3REGISTRY_H
#define LAYER3REGISTRY_H

#include <functional>
#include <vector>
#include <packet/PacketUtil.h>
#include <layerx/Layer.h>

struct Layer3Registry {
    static std::vector<std::pair<const int, Layer::function>>& get_registry() {
        static std::vector<std::pair<const int, Layer::function>> registry;
        return registry;
    }

    Layer3Registry(int key, Layer::function func) {
        get_registry().push_back(std::pair{key, func});
    }

};

#endif //LAYER3REGISTRY_H
