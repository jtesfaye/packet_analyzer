//
// Created by jeremiah tesfaye on 10/23/25.
//

#ifndef DETAILPARSE_H
#define DETAILPARSE_H

#include <vector>
#include <packet/PacketUtil.h>

using namespace packet;

class DetailParse {
public:
    DetailParse() = default;
    ProtocolDetails extract_details(const std::vector<std::byte>, LayerRegion offsets);
};

#endif //DETAILPARSE_H
