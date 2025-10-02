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

    static std::unique_ptr<NetworkPDU> unsupported_type(
        const std::vector<std::byte>&,
        packet::parse_context&);

    static const std::vector<std::pair<const int, function>> get_all_functions();

};

#endif //LAYER3_H
