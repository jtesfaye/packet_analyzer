//
// Created by jeremiah tesfaye on 9/18/25.
//

#include <layerx/layer2/Layer2.h>
#include <layerx/layer2/Ethernet.h>

std::unique_ptr<LinkPDU> Layer2::unsupported_type(const std::vector<std::byte> &, parse_context &) {
    return nullptr;
}


const std::vector<std::pair<const int, Layer2::function>> &Layer2::get_all_functions() {

    (void)ethernet_functions::get_ethernet_registry();
    Layer2Registry(-1, unsupported_type);

    return Layer2Registry::get_registry();
}
