//
// Created by jeremiah tesfaye on 9/18/25.
//

#ifndef LAYER4REGISTRY_H
#define LAYER4REGISTRY_H

#include <functional>
#include <vector>
#include <layerx/Layer.h>

struct Layer4Registry {
    static std::vector<std::pair<const int, Layer::function>>& get_registry() {
        static std::vector<std::pair<const int, Layer::function>> registry;
        return registry;
    }

    Layer4Registry(int key, Layer::function func) {
        get_registry().push_back(std::pair{key, func});
    }

};

#endif //LAYER4REGISTRY_H
