//
// Created by jeremiah tesfaye on 9/3/25.
//

#ifndef LAYER4_H
#define LAYER4_H

#include <vector>
#include <functional>
#include <packet/LayerUtil.h>
#include <layer4_protocols/Layer4Types.h>
#include <layer4_protocols/TCP.h>

using namespace layer::transport;
using namespace packet;

class Layer4 {

    using function = std::function<std::unique_ptr<TransportPDU>(const std::vector<std::byte>&, parse_context&)>;
    using key_pair = std::pair<int, function>;

public:

    Layer4() = delete;
    Layer4(const Layer4&) = delete;
    Layer4& operator= (const Layer4&) = delete;

    static auto get_all_functions() {return all_functions;}

private:

    inline static const std::initializer_list<key_pair> all_functions {
        key_pair {TCP_IANA, function(tcp_functions::tcp_parse)}
    };


};


#endif //LAYER4_H
