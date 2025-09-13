//
// Created by jeremiah tesfaye on 9/3/25.
//

#ifndef LAYER3_H
#define LAYER3_H

#include <vector>
#include <functional>
#include <layerx/layer3/Layer3Types.h>
#include <layerx/iana_numbers.h>
#include <layerx/layer3/IPv4.h>
#include <packet/PacketUtil.h>

using namespace layer;
class Layer3 {

    using function = std::function<std::unique_ptr<NetworkPDU>>(
        const std::vector<std::byte>&,
        packet::parse_context&);

    using key_pair = std::pair<int, function>;

public:

    Layer3() = delete;
    Layer3(const Layer3&) = delete;
    Layer3& operator= (const Layer3&) = delete;

    static std::initializer_list<key_pair> get_all_functions() {return all_functions;}

private:

    inline static const std::initializer_list<key_pair> all_functions {
        key_pair {iana::IPV4, function(IPv4_functions::IPv4_parse)}
    };

};

#endif //LAYER3_H
