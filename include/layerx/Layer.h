//
// Created by jeremiah tesfaye on 10/2/25.
//

#ifndef LAYER_H
#define LAYER_H
#include <packet/PacketUtil.h>
#include <functional>
#include <layerx/ProtocolDataUnit.h>
#include <concepts>

using namespace packet;

class Layer {
public:

    using function = std::function<std::unique_ptr<ProtocolDataUnit>(
        const std::vector<std::byte>&,
        parse_context&)>;

    using detail_function = std::function<ProtocolDetails(
        const std::vector<std::byte>&,
        parse_context&)>;

    using key_pair = std::pair<int, function>;

    static bool registered;

    static std::unique_ptr<ProtocolDataUnit>
    unregistered_type(const std::vector<std::byte> &, parse_context &);

    static ProtocolDetails
    unregistered_type_details(const std::vector<std::byte> &, parse_context &);

    static std::vector<std::pair<int, function>> first_parse_funcs;

    static std::vector<std::pair<int, detail_function>> detail_parse_funcs;

    static void register_parse_functions();

    Layer() = delete;
    Layer(const Layer&) = delete;
    Layer& operator= (const Layer&) = delete;

    virtual ~Layer() = default;

    static std::vector<std::pair<int, function>> get_first_parse_functions();

    static std::vector<std::pair<int, detail_function>> get_detail_parse_functions();

private:

    template<typename T>
    static void append_func_vec(T& dest, const T& src) {

        dest.insert(dest.end(), src.begin(), src.end());
    }
};

#endif //LAYER_H
