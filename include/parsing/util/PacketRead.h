//
// Created by jeremiah tesfaye on 5/20/25.
//

#ifndef PACKETREAD_H
#define PACKETREAD_H

#include <packet/ProtocolTypes.h>
#include "PacketParse.h"

using namespace packet;
using namespace packet::frame;

class PacketRead {
public:

    explicit PacketRead();

    ~PacketRead() = default;

    static link_layer copy(link_layer_ref&) ;

    static std::vector<std::string> create_row_entry(packet_ref&);

    static std::string format_mac(const u_int8_t* addr);

    template<typename T>
    static T read_as(const link_layer_ref& l2) {

        return *std::get<T*>(l2);

    }

    static std::vector<std::byte> copy_packet(const u_int8_t*, size_t len);

};



#endif //PACKETREAD_H
