//
// Created by jeremiah tesfaye on 9/16/25.
//

#ifndef ROWFACTORY_H
#define ROWFACTORY_H

#include <string>
#include <array>
#include <layerx/ProtocolDataUnit.h>
#include <packet/PacketUtil.h>

struct row_entry {

    std::string index;
    std::string time;
    std::string src;
    std::string dest;
    std::string protocol;
    std::string length;
    std::string info;

    std::array<const std::string*, 7> to_array() {
        return {&index, &time, &src, &dest, &protocol, &length, &info};
    }
};

class RowFactory {
public:

    static row_entry create_row(packet::packet_ref& ref);

    RowFactory() = delete;

private:

    static row_entry layer4_top_row(
        size_t index,
        double time,
        size_t length,
        const TransportPDU* data,
        const NetworkPDU* addr_data);

    static row_entry layerx_top_row(
        size_t index,
        double time,
        size_t length,
        const ProtocolDataUnit* data);

};

#endif //ROWFACTORY_H
