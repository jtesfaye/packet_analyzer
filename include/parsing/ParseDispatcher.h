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
    using pair = std::pair<const int, function>;

    template<bool F = Fixed, typename = std::enable_if_t<F>>
    explicit ParseDispatcher(int key, std::vector<pair> funcs)
    {

        func_table = std::unordered_map<int, function>(funcs.begin(), funcs.end());
        auto k = func_table.find(key);

        if (k == func_table.end()) {

            parse_func = func_table.at(-1);

        } else {

            parse_func = func_table.at(key);

        }
    }

    template<bool F = Fixed, typename = std::enable_if_t<!F>>
    explicit ParseDispatcher(std::vector<pair> funcs) {
        func_table = std::unordered_map<int, function>(funcs.begin(), funcs.end());
    }

    template<bool F = Fixed, typename = std::enable_if_t<F>>
    RefType operator() (const std::vector<std::byte> &raw_data, parse_context& context) const {

        return parse_func(raw_data, context);

    }

    template<bool F = Fixed, typename = std::enable_if_t<!F>>
    RefType operator() (int key, const std::vector<std::byte> &raw_data, parse_context& context) {

        if (auto k = func_table.find(key) == func_table.end()) {

            return func_table.at(-1)(raw_data, context);

        }

        return func_table.at(key)(raw_data, context);
    }

private:

    std::unordered_map<int, function> func_table;
    std::function<RefType(const std::vector<std::byte>&, parse_context&)> parse_func;

};

#endif //PARSEDISPATCHER_H
