//
// Created by jeremiah tesfaye on 8/31/25.
//

#ifndef LAYER2_H
#define LAYER2_H

#include <vector>
#include <functional>
#include <layerx/layer2/Layer2Types.h>
#include <packet/PacketUtil.h>

using namespace packet;
class Layer2 {

    using function = std::function<std::unique_ptr<LinkPDU>(
        const std::vector<std::byte>&,
        parse_context&)>;

    using key_pair = std::pair<const int, function>;

public:

    Layer2() = delete;
    Layer2(const Layer2&) = delete;
    Layer2& operator= (const Layer2&) = delete;

    static std::unique_ptr<LinkPDU> unsupported_type(
        const std::vector<std::byte>&,
        parse_context&);

    static const std::vector<std::pair<const int, function>>& get_all_functions();

};


#endif //LAYER2_H
