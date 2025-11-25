//
// Created by jeremiah tesfaye on 11/19/25.
//

#ifndef PARSEFORMAT_H
#define PARSEFORMAT_H

#include <packet/PacketUtil.h>

namespace parse {

    using function = std::function<std::unique_ptr<ProtocolDataUnit>(
        std::span<std::byte>,
        packet::parse_context&)>;

    using detail_function = std::function<packet::ProtocolDetails(
        std::span<std::byte>,
        packet::parse_context&)>;

    using key_pair = std::pair<int, function>;

};




#endif //PARSEFORMAT_H
