//
// Created by jeremiah tesfaye on 9/3/25.
//

#ifndef LAYER4_H
#define LAYER4_H

#include <vector>
#include <functional>
#include <layerx/layer4/TCP.h>
#include <layerx/iana_numbers.h>
#include <packet/PacketUtil.h>

class Layer4 {

    using function = std::function<std::unique_ptr<TransportPDU>(
        const std::vector<std::byte>&,
        parse_context&)>;

    using key_pair = std::pair<const int, function>;

public:

    Layer4() = delete;
    Layer4(const Layer4&) = delete;
    Layer4& operator= (const Layer4&) = delete;

    static std::unique_ptr<TransportPDU> unsupported_layer(
        const std::vector<std::byte>&,
        parse_context&) {return nullptr;}

    static std::initializer_list<key_pair> get_all_functions() {return all_functions;}

private:

    inline static const std::initializer_list<key_pair> all_functions {
        key_pair {-1, unsupported_layer},
        key_pair {layer::iana::TCP, tcp_functions::tcp_parse}
    };


};


#endif //LAYER4_H
