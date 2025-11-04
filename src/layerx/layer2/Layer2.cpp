//
// Created by jeremiah tesfaye on 9/18/25.
//

#include <layerx/layer2/Layer2.h>
#include <layerx/layer2/Ethernet.h>


void Layer2::register_all_functions() {

    ethernet_functions::register_ethernet();

}

const std::vector<std::pair<int, Layer::function>> &Layer2::get_first_parse_registry() {
    return Layer2Registry::get_registry();
}

const std::vector<std::pair<int, Layer::detail_function> > &Layer2::get_detail_parse_registry() {
    return Layer2Registry::get_detail_registry();
}

