//
// Created by jeremiah tesfaye on 9/18/25.
//

#include <layerx/layer4/Layer4.h>
#include <layerx/layer4/TCP.h>
#include <layerx/layer4/ICMP.h>
#include <layerx/layer4/UDP.h>

std::unique_ptr<TransportPDU> Layer4::unsupported_type(
    const std::vector<std::byte>&,
    packet::parse_context&) {
    return nullptr;
}

void Layer4::register_all_functions() {

    tcp_functions::register_tcp();
    icmp_functions::get_icmp_registry();
    udp_functions::get_udp_registry();
    Layer4Registry(-1, unsupported_type);

}

const std::vector<std::pair<int, Layer4::function>>& Layer4::get_first_parse_registry() {
    return Layer4Registry::get_registry();
}

const std::vector<std::pair<int, Layer::detail_function> > &Layer4::get_detail_parse_registry() {
    return  Layer4Registry::get_detail_registry();
}



