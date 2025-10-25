//
// Created by jeremiah tesfaye on 9/3/25.
//

#ifndef LAYER4_H
#define LAYER4_H

#include <layerx/layer4/Layer4Types.h>
#include <layerx/Layer.h>

using namespace packet;

class Layer4 : public Layer {
public:

    static std::unique_ptr<TransportPDU> unsupported_type(
        const std::vector<std::byte>&,
        packet::parse_context&);

    static void register_all_functions();

    static const std::vector<std::pair<int, function>> &get_first_parse_registry();
    static const std::vector<std::pair<int, detail_function>> &get_detail_parse_registry();

    ~Layer4() override = default;

};


#endif //LAYER4_H
