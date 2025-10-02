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

    static std::unique_ptr<LinkPDU> unsupported_type(
        const std::vector<std::byte>&,
        parse_context&);

    static const std::vector<std::pair<const int, function>>& get_all_functions();

};


#endif //LAYER2_H
