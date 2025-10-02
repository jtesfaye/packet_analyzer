//
// Created by jeremiah tesfaye on 10/2/25.
//

#ifndef LAYER_H
#define LAYER_H
#include <packet/PacketUtil.h>
#include <functional>
#include <layerx/ProtocolDataUnit.h>

class Layer {
public:
    using function = std::function<std::unique_ptr<ProtocolDataUnit>(
    const std::vector<std::byte>&,
    packet::parse_context&)>;

    using key_pair = std::pair<const int, function>;

    Layer() = delete;
    Layer(const Layer&) = delete;
    Layer& operator= (const Layer&) = delete;

};

#endif //LAYER_H
