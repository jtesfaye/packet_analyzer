//
// Created by jeremiah tesfaye on 9/3/25.
//

#ifndef LAYER3_H
#define LAYER3_H

#include <layerx/layer3/Layer3Types.h>
#include <layerx/Layer.h>

using namespace layer;
class Layer3 : public Layer {
public:

    static void register_all_functions();

    static const std::vector<std::pair<int, function>>& get_first_parse_registry();

    static const std::vector<std::pair<int, detail_function>> &get_detail_parse_registry();

    ~Layer3() override = default;
};

#endif //LAYER3_H
