//
// Created by jeremiah tesfaye on 8/31/25.
//

#ifndef LAYER2_H
#define LAYER2_H

#include <pcap/pcap.h>
#include <vector>
#include <functional>
#include <layerx/layer2/Ethernet.h>
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

    static std::initializer_list<key_pair> get_all_functions() {return all_layer2_functions;}

private:

    inline static const std::initializer_list<key_pair> all_layer2_functions {
        key_pair {DLT_EN10MB, ethernet_functions::ethernet_parse}
    };

};


#endif //LAYER2_H
