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

    static void register_all_functions();

    static const std::vector<std::pair<int, function>> &get_first_parse_registry();
    static const std::vector<std::pair<int, detail_function>> &get_detail_parse_registry();

    ~Layer4() override = default;

};

struct TransportPDU : ProtocolDataUnit {

    TransportPDU(size_t len, u_int16_t src, u_int16_t dest)
    : ProtocolDataUnit(len, src, dest)
    {}

    ~TransportPDU() override = default;

    size_t stream_index = -1;

};


#endif //LAYER4_H
