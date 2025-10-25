//
// Created by jeremiah tesfaye on 8/31/25.
//

#ifndef PARSEDISPATCHER_H
#define PARSEDISPATCHER_H

#include <functional>
#include <packet/PacketUtil.h>

using namespace packet;

template <typename RefType>
class ParseDispatcher {
public:

    using function = std::function<RefType(const std::vector<std::byte>&, parse_context&)>;
    using pair = std::pair<int, function>;

    explicit ParseDispatcher(std::vector<pair> funcs) {
        func_table = std::unordered_map<int, function>(funcs.begin(), funcs.end());
    }

    RefType operator() (int key, const std::vector<std::byte> &raw_data, parse_context& context) {

        if (auto k = !func_table.contains(key)) {

            return func_table.at(-1)(raw_data, context);

        }

        return func_table.at(key)(raw_data, context);
    }

private:

    std::unordered_map<int, function> func_table;
    std::function<RefType(const std::vector<std::byte>&, parse_context&)> parse_func;

};

#endif //PARSEDISPATCHER_H
