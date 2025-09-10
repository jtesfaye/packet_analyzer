//
// Created by jeremiah tesfaye on 9/3/25.
//

#ifndef LAYER3_H
#define LAYER3_H

#include <vector>
#include <functional>
#include <packet/LayerWrappers.h>
#include <layer3_protocols/IPv4.h>
#include <layer3_protocols/Layer3Types.h>

using namespace packet::ip;
class Layer3 {

    using function = std::function<std::unique_ptr<NetworkPDU>(const std::vector<std::byte>&, parse_context&)>;
    using key_pair = std::pair<int, function>;

public:

    Layer3() = delete;
    Layer3(const Layer3&) = delete;
    Layer3& operator= (const Layer3&) = delete;

    static auto get_all_functions() {return all_functions;}

private:

    inline static const std::initializer_list<key_pair> all_functions {
        key_pair {NT_IPV4, function(IPv4_functions::IPv4_parse)}
    };

};

#endif //LAYER3_H
