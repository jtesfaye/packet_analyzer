//
// Created by jeremiah tesfaye on 5/20/25.
//

#ifndef PACKETREAD_H
#define PACKETREAD_H

#include <string>

class PacketRead {
public:

    explicit PacketRead();

    ~PacketRead() = default;

    static std::string format_mac(const u_int8_t* addr);

    static std::string format_ipv4_src_dst(const std::byte*);


};



#endif //PACKETREAD_H
