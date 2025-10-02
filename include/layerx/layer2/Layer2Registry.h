//
// Created by jeremiah tesfaye on 9/17/25.
//

#ifndef LAYER2REGISTRY_H
#define LAYER2REGISTRY_H
#include <functional>
#include <vector>
#include <layerx/Layer.h>


struct Layer2Registry {
    static std::vector<std::pair<const int, Layer::function>>& get_registry() {
        static std::vector<std::pair<const int, Layer::function>> registry;
        return registry;
    }

    Layer2Registry(int key, Layer::function func) {
        get_registry().push_back(std::pair{key, func});
    }

};
#endif //LAYER2REGISTRY_H
