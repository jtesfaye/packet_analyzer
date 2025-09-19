//
// Created by jeremiah tesfaye on 9/3/25.
//

#ifndef LAYER3_H
#define LAYER3_H

#include <vector>
#include <functional>
#include <layerx/layer3/Layer3Types.h>
#include <packet/PacketUtil.h>

using namespace layer;
class Layer3 {

    using function = std::function<std::unique_ptr<NetworkPDU>(
        const std::vector<std::byte>&,
        packet::parse_context&)>;

    using key_pair = std::pair<const int, function>;

public:

    Layer3() = delete;
    Layer3(const Layer3&) = delete;
    Layer3& operator= (const Layer3&) = delete;

    static std::unique_ptr<NetworkPDU> unsupported_type(
        const std::vector<std::byte>&,
        packet::parse_context&);

    static const std::vector<std::pair<const int, function>> get_all_functions();

};

#endif //LAYER3_H
