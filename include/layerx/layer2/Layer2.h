//
// Created by jeremiah tesfaye on 8/31/25.
//

#ifndef LAYER2_H
#define LAYER2_H

#include <layerx/layer2/Layer2Types.h>
#include <layerx/Layer.h>

using namespace packet;

class Layer2 : public Layer {
public:

    static void register_all_functions();

    static const std::vector<std::pair<int, function>>& get_first_parse_registry();

    static const std::vector<std::pair<int, detail_function>>& get_detail_parse_registry();

    ~Layer2() override = default;
};


#endif //LAYER2_H
