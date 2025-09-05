//
// Created by jeremiah tesfaye on 7/27/25.
//

#include "../../../include/layer2_protocols/EN10MB.h"


link_layer_ref
EN10MB_functions::EN10MB_parse
(const u_int8_t* raw_data, parse_context& context) {

    using namespace packet::frame;
    using namespace packet;

    auto ether = reinterpret_cast<const EN10MB*> (raw_data);

    if (ntohs(ether->ether_type) == 0x8100) {

        auto vlan = reinterpret_cast<const EN10MB_802_1_Q*> (raw_data);

        context.length = 18;
        context.layer3_type = ntohs(vlan->ether_type);

        return  link_layer_ref {const_cast<EN10MB_802_1_Q*>(vlan)};
    }

    context.length = 14;
    context.layer3_type = ntohs(ether->ether_type);

    return link_layer_ref {const_cast<EN10MB*>(ether)};

}
