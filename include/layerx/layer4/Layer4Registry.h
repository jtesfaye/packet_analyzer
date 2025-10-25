//
// Created by jeremiah tesfaye on 9/18/25.
//

#ifndef LAYER4REGISTRY_H
#define LAYER4REGISTRY_H

#include <functional>
#include <vector>
#include <layerx/Layer.h>

struct Layer4Registry {

    static std::vector<std::pair<int, Layer::function>>& get_registry() {
        static std::vector<std::pair<int, Layer::function>> registry;
        return registry;
    }

    static std::vector<std::pair<int, Layer::detail_function>>& get_detail_registry() {
        static std::vector<std::pair<int ,Layer::detail_function>> detail_reg;
        return detail_reg;
    }

    Layer4Registry(int key, const Layer::function& func) {
        get_registry().push_back(std::pair{key, func});
    }

    Layer4Registry(int key, const Layer::detail_function& func) {
        get_detail_registry().push_back(std::pair{key, func});
    }

};

#endif //LAYER4REGISTRY_H
