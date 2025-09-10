//
// Created by jeremiah tesfaye on 8/31/25.
//

#ifndef LAYER2_H
#define LAYER2_H

#include <vector>
#include <functional>
#include <layer2_protocols/Layer2Types.h>
#include <layer2_protocols/Ethernet.h>

class Layer2 {

    using function = std::function<std::unique_ptr<LinkPDU>(const std::vector<std::byte>&, parse_context&)>;
    using key_pair = std::pair<int, function>;

public:

    Layer2() = delete;
    Layer2(const Layer2&) = delete;
    Layer2& operator= (const Layer2&) = delete;

    static auto get_all_functions() {return all_layer2_functions;}

private:

    inline static const std::initializer_list<key_pair> all_layer2_functions {
        key_pair {DLT_EN10MB, function(Ethernet_functions::ethernet_parse)}
    };

};


#endif //LAYER2_H
