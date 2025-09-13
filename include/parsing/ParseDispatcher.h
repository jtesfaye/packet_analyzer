//
// Created by jeremiah tesfaye on 8/31/25.
//

#ifndef PARSEDISPATCHER_H
#define PARSEDISPATCHER_H

#include <functional>
#include <packet/PacketUtil.h>

using namespace packet;

template <typename RefType, bool Fixed>
class ParseDispatcher {
public:

    using function = std::function<RefType(const std::vector<std::byte>&, parse_context&)>;

    template<bool F = Fixed, typename = std::enable_if_t<F>>
    explicit ParseDispatcher(int key, std::initializer_list<std::pair<int, function>> funcs)
        : func_table(funcs)
        , parse_func(func_table.at(key))
    {}

    template<bool F = Fixed, typename = std::enable_if_t<!F>>
    ParseDispatcher(std::initializer_list<std::pair<int, function>> funcs)
        : func_table(funcs)
        , parse_func()
    {}

    template<bool F = Fixed, typename = std::enable_if_t<F>>
    RefType operator() (const std::vector<std::byte> &raw_data, parse_context& context) const {
        return parse_func(raw_data, context);
    }

    template<bool F = Fixed, typename = std::enable_if_t<!F>>
    RefType operator() (int key, const std::vector<std::byte> &raw_data, parse_context& context) {

        return func_table.at(key)(raw_data, context);
    }

private:

    std::unordered_map<int, function> func_table;
    std::function<RefType(const std::vector<std::byte>&, parse_context&)> parse_func;



};

#endif //PARSEDISPATCHER_H
