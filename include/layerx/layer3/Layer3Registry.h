//
// Created by jeremiah tesfaye on 9/18/25.
//

#ifndef LAYER3REGISTRY_H
#define LAYER3REGISTRY_H

#include <functional>
#include <vector>
#include <layerx/ParseFormat.h>

namespace registry::layer3 {

    inline std::vector<std::pair<int, parse::function>> layer3_initial_parse_registry;
    inline std::vector<std::pair<int, parse::detail_function>> layer3_detail_parse_registry;

    void register_self(int key, const parse::function& func);

    void register_self(int key, const parse::detail_function& func);

    void register_all_functions();

}



#endif //LAYER3REGISTRY_H
