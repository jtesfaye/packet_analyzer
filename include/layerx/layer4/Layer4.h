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

    static const std::vector<std::pair<const int, function>> get_all_functions();

};


#endif //LAYER4_H
