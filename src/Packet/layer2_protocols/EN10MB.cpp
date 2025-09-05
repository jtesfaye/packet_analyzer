//
// Created by jeremiah tesfaye on 7/27/25.
//

#include <layer2_protocols/EN10MB.h>

link_layer_ref
EN10MB_functions::EN10MB_parse
(const std::vector<std::byte> &raw_data, parse_context& context) {

    using namespace packet::frame;
    using namespace packet;

    if (raw_data.size() < sizeof(EN10MB)) {
        throw std::runtime_error("Packet too short for ethernet header");
    }

    const std::byte* layer2_start = raw_data.data() + context.offset;

    auto ether = reinterpret_cast<const EN10MB*> (layer2_start);

    if (ntohs(ether->ether_type) == 0x8100) {

        auto vlan = reinterpret_cast<const EN10MB_802_1_Q*> (layer2_start);

        context.length = 18;
        context.next_type = ntohs(vlan->ether_type);

        return  link_layer_ref {static_cast<EN10MB_802_1_Q*>(nullptr)};
    }

    if (ntohs(ether->ether_type) == 0x88A8) {

        auto q_ad = reinterpret_cast<const EN10MB_802_1_ad*> (layer2_start);
        context.length = 22;
        context.next_type = ntohs(q_ad->ether_type);

        return link_layer_ref {static_cast<EN10MB_802_1_ad*> (nullptr)};

    }

    context.length = 14;
    context.next_type = ntohs(ether->ether_type);

    return link_layer_ref {static_cast<EN10MB*>(nullptr)};

}
