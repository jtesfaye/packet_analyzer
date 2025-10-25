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
    static std::vector<std::pair<int, Layer::function>>& get_registry() {
        static std::vector<std::pair<int, Layer::function>> registry;
        return registry;
    }

    static std::vector<std::pair<int, Layer::detail_function>>& get_detail_registry() {
        static std::vector<std::pair<int ,Layer::detail_function>> detail_reg;
        return detail_reg;
    }


    Layer3Registry(int key, const Layer::function& func) {
        get_registry().push_back(std::pair{key, func});
    }

    Layer3Registry(int key, const Layer::detail_function& func) {
        get_detail_registry().push_back(std::pair{key, func});
    }

};

#endif //LAYER3REGISTRY_H
