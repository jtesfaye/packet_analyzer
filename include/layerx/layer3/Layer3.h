//
// Created by jeremiah tesfaye on 9/3/25.
//

#ifndef LAYER3_H
#define LAYER3_H

#include <vector>
#include <functional>
#include <layerx/iana_numbers.h>
#include <layerx/layer3/IPv4.h>

using namespace layer;
class Layer3 {

    using function = std::function<std::unique_ptr<NetworkPDU>(
        const std::vector<std::byte>&,
        parse_context&)>;

    using key_pair = std::pair<const int, function>;

public:

    Layer3() = delete;
    Layer3(const Layer3&) = delete;
    Layer3& operator= (const Layer3&) = delete;

    static std::unique_ptr<NetworkPDU> unsupported_layer(
        const std::vector<std::byte>&,
        parse_context&) {return nullptr;}

    static std::initializer_list<key_pair> get_all_functions() {return all_functions;}

private:


    inline static const std::initializer_list<key_pair> all_functions {
        key_pair {-1, unsupported_layer},
        key_pair {iana::IPV4, IPv4_functions::ipv4_parse}
    };

};

#endif //LAYER3_H
