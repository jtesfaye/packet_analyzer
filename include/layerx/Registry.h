//
// Created by jeremiah tesfaye on 10/2/25.
//

#ifndef LAYER_H
#define LAYER_H

#include <layerx/layer2/Layer2Registry.h>
#include <layerx/layer3/Layer3Registry.h>
#include <layerx/layer4/Layer4Registry.h>

using namespace packet;

namespace registry {

    template<typename T>
    static void append_func_vec(T& dest, const T& src) {
        dest.insert(dest.end(), src.begin(), src.end());
    }

    inline static std::vector<std::pair<int, parse::function>> full_initial_reg;

    inline static std::vector<std::pair<int, parse::detail_function>> full_detail_reg;

    inline std::vector<std::pair<int, parse::function>> get_initial_registry() {

        if (layer2::layer2_initial_parse_registry.empty()) {
            layer2::register_all_functions();
            append_func_vec(full_initial_reg, layer2::layer2_initial_parse_registry);
        }

        if (layer3::layer3_initial_parse_registry.empty()) {
            layer3::register_all_functions();
            append_func_vec(full_initial_reg, layer3::layer3_initial_parse_registry);
        }

        if (layer4::layer4_initial_parse_registry.empty()) {
            layer4::register_all_functions();
            append_func_vec(full_initial_reg, layer4::layer4_initial_parse_registry);
        }

        return full_initial_reg;

    }

    inline std::vector<std::pair<int, parse::detail_function>> get_detail_parse_functions() {

        if (layer2::layer2_detail_parse_registry.empty()) {
            layer2::register_all_functions();
            append_func_vec(full_detail_reg, layer2::layer2_detail_parse_registry);
        }

        if (layer3::layer3_detail_parse_registry.empty()) {
            layer3::register_all_functions();
            append_func_vec(full_detail_reg, layer3::layer3_detail_parse_registry);

        }

        if (layer4::layer4_initial_parse_registry.empty()) {
            layer4::register_all_functions();
            append_func_vec(full_detail_reg, layer4::layer4_detail_parse_registry);
        }

        return full_detail_reg;
    }
}

#endif //LAYER_H
