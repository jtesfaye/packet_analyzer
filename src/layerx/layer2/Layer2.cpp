//
// Created by jeremiah tesfaye on 9/18/25.
//

#include <layerx/layer2/Layer2.h>
#include <layerx/layer2/Ethernet.h>

std::unique_ptr<LinkPDU> Layer2::unsupported_type(const std::vector<std::byte> &, parse_context &) {
    return nullptr;
}


void Layer2::register_all_functions() {

    (void)ethernet_functions::get_ethernet_registry();
    Layer2Registry(-1, unsupported_type);

}

const std::vector<std::pair<int, Layer::function>> &Layer2::get_first_parse_registry() {
    return Layer2Registry::get_registry();
}

const std::vector<std::pair<int, Layer::detail_function> > &Layer2::get_detail_parse_registry() {
    return Layer2Registry::get_detail_registry();
}

