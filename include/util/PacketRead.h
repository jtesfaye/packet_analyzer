//
// Created by jeremiah tesfaye on 5/20/25.
//

#ifndef PACKETREAD_H
#define PACKETREAD_H

#include <string>
#include <span>
class PacketRead {
public:

    PacketRead();
    ~PacketRead() = default;

    static bool valid_length(std::span<std::byte> data, size_t offset, size_t needed);

    static std::string format_mac(const u_int8_t* addr);

    static std::string format_ipv4_src_dst(const u_int32_t& addr);

    static std::string format_ipv6_src_dest(const u_int8_t* addr);

};



#endif //PACKETREAD_H
