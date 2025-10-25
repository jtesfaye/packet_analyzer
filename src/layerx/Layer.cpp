//
// Created by jeremiah tesfaye on 10/2/25.
//

#include <layerx/Layer.h>
#include <layerx/layer4/Layer4.h>
#include <layerx/layer3/Layer3.h>
#include <layerx/layer2/Layer2.h>

bool Layer::registered = false;

std::vector<std::pair<int, Layer::function>> Layer::first_parse_funcs;

std::vector<std::pair<int, Layer::detail_function>> Layer::detail_parse_funcs;

void Layer::register_parse_functions() {

    if (registered) return;

    Layer4::register_all_functions();
    append_func_vec(first_parse_funcs, Layer4::get_first_parse_registry());
    append_func_vec(detail_parse_funcs, Layer4::get_detail_parse_registry());

    Layer3::register_all_functions();
    append_func_vec(first_parse_funcs, Layer3::get_first_parse_registry());
    append_func_vec(detail_parse_funcs, Layer3::get_detail_parse_registry());

    Layer2::register_all_functions();
    append_func_vec(first_parse_funcs, Layer2::get_first_parse_registry());
    append_func_vec(detail_parse_funcs, Layer2::get_detail_parse_registry());

    registered = true;

}

std::vector<std::pair<int, Layer::function> > Layer::get_first_parse_functions() {
    return first_parse_funcs;
}

std::vector<std::pair<int, Layer::detail_function> > Layer::get_detail_parse_functions() {
    return detail_parse_funcs;
}

