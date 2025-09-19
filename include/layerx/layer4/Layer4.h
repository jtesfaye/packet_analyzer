//
// Created by jeremiah tesfaye on 9/3/25.
//

#ifndef LAYER4_H
#define LAYER4_H

#include <vector>
#include <functional>
#include <packet/PacketUtil.h>
#include <layerx/layer4/Layer4Types.h>

class Layer4 {

    using function = std::function<std::unique_ptr<TransportPDU>(
        const std::vector<std::byte>&,
        packet::parse_context&)>;

    using key_pair = std::pair<const int, function>;

public:

    Layer4() = delete;
    Layer4(const Layer4&) = delete;
    Layer4& operator= (const Layer4&) = delete;

    static std::unique_ptr<TransportPDU> unsupported_layer(
        const std::vector<std::byte>&,
        packet::parse_context&);

    static const std::vector<std::pair<const int, function>> get_all_functions();


};


#endif //LAYER4_H
