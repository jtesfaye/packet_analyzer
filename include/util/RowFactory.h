//
// Created by jeremiah tesfaye on 9/16/25.
//

#ifndef ROWFACTORY_H
#define ROWFACTORY_H

#include <string>
#include <array>
#include <layerx/ProtocolDataUnit.h>
#include <packet/PacketUtil.h>

class RowFactory {
public:

    struct row_entry {
        size_t index;
        double time;
        std::string src;
        std::string dest;
        std::string protocol;
        size_t length;
        std::string info;

    };

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
