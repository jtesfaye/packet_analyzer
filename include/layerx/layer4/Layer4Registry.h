//
// Created by jeremiah tesfaye on 9/18/25.
//

#ifndef LAYER4REGISTRY_H
#define LAYER4REGISTRY_H

#include <functional>
#include <vector>
#include <layerx/ParseFormat.h>

namespace registry::layer4 {

    inline std::vector<std::pair<int, parse::function>> layer4_initial_parse_registry;
    inline std::vector<std::pair<int, parse::detail_function>> layer4_detail_parse_registry;

    void register_self(int key, const parse::function& func);

    void register_self(int key, const parse::detail_function& func);

    void register_all_functions();

}

#endif //LAYER4REGISTRY_H
