//
// Created by jeremiah tesfaye on 9/17/25.
//

#ifndef LAYER2REGISTRY_H
#define LAYER2REGISTRY_H
#include <functional>
#include <vector>
#include <layerx/ParseFormat.h>


namespace registry::layer2 {
    inline std::vector<std::pair<int, parse::function>> layer2_initial_parse_registry;
    inline std::vector<std::pair<int, parse::detail_function>> layer2_detail_parse_registry;

    void register_self(int key, const parse::function& func);

    void register_self(int key, const parse::detail_function& func);

    void register_all_functions();
}

#endif //LAYER2REGISTRY_H
