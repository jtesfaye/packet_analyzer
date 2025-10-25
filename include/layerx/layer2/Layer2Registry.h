//
// Created by jeremiah tesfaye on 9/17/25.
//

#ifndef LAYER2REGISTRY_H
#define LAYER2REGISTRY_H
#include <functional>
#include <vector>
#include <layerx/Layer.h>


struct Layer2Registry {
    static std::vector<std::pair<int, Layer::function>>& get_registry() {
        static std::vector<std::pair<int, Layer::function>> registry;
        return registry;
    }

    static std::vector<std::pair<int, Layer::detail_function>>& get_detail_registry() {
        static std::vector<std::pair<int ,Layer::detail_function>> detail_reg;
        return detail_reg;
    }


    Layer2Registry(int key, const Layer::function& func) {
        get_registry().push_back(std::pair{key, func});
    }

    Layer2Registry(int key, const Layer::detail_function& func) {
        get_detail_registry().push_back(std::pair{key, func});
    }


};
#endif //LAYER2REGISTRY_H
