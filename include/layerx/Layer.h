//
// Created by jeremiah tesfaye on 10/2/25.
//

#ifndef LAYER_H
#define LAYER_H
#include <packet/PacketUtil.h>
#include <functional>
#include <layerx/ProtocolDataUnit.h>
#include <concepts>
#include <span>

using namespace packet;

class Layer {
public:

    using function = std::function<std::unique_ptr<ProtocolDataUnit>(
        std::span<std::byte>,
        parse_context&)>;

    using detail_function = std::function<ProtocolDetails(
        std::span<std::byte>,
        parse_context&)>;

    using key_pair = std::pair<int, function>;

    static std::vector<std::pair<int, function>> get_first_parse_functions();

    static std::vector<std::pair<int, detail_function>> get_detail_parse_functions();

    Layer() = delete;
    Layer(const Layer&) = delete;
    Layer& operator= (const Layer&) = delete;
    virtual ~Layer() = default;

private:

    static std::unique_ptr<ProtocolDataUnit> unregistered_type(std::span<std::byte>, parse_context &);

    static ProtocolDetails unregistered_type_details(std::span<std::byte>, parse_context &);

    static void register_parse_functions();

    static bool registered;

    static std::vector<std::pair<int, function>> first_parse_funcs;

    static std::vector<std::pair<int, detail_function>> detail_parse_funcs;

    template<typename T>
    static void append_func_vec(T& dest, const T& src) {

        dest.insert(dest.end(), src.begin(), src.end());
    }
};

#endif //LAYER_H
