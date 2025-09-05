//
// Created by jeremiah tesfaye on 5/20/25.
//



#include <parsing/util/PacketRead.h>
#include <util/Overload.h>
#include <packet/LayerWrappers.h>

#include <sstream>
#include <iomanip>
#include <iostream>
#include <optional>

//We are working on making linkread generic, and now trying to implement into this func

PacketRead::PacketRead() = default;

link_layer
PacketRead::copy(link_layer_ref& l2) {

    return std::visit(Overload{

        [&](std::monostate) -> link_layer {

            return {};

        },

        [&](auto* ptr) -> link_layer {

            using T = std::remove_pointer_t<decltype(ptr)>;
            return T{*ptr};

        }

    }, l2);

}


std::vector<std::string>
PacketRead::create_row_entry(packet_ref& packet) {

    using source_dest = std::pair<std::string, std::string>;

    std::string protocol;
    std::string info = "n/a";

    //if no layer 3, then use layer2 for src and dst.
    if (packet.l3.index() == 0) {

        std::optional<source_dest> src_dest = std::visit(Overload{

            [&](EN10MB* eth) -> std::optional<source_dest> {
                if (!eth) {
                    protocol = "n/a";
                    return std::nullopt;
                }

                protocol = "Ethernet";

                return std::make_pair(format_mac(eth->src_addr), format_mac(eth->dest_addr));
            },

            [&](EN10MB_802_1_Q* eth) -> std::optional<source_dest> {

                if (!eth) {
                    protocol = "n/a";
                    return std::nullopt;
                }
                    protocol = "802.1Q";
                    return std::make_pair(format_mac(eth->src_addr), format_mac(eth->dest_addr));
            },

             [&](auto*) -> std::optional<source_dest> {
                protocol = "n/a";
                return std::nullopt;

            },

            [&](std::monostate) -> std::optional<source_dest> {
                protocol = "n/a";
                return std::nullopt;
            },

        }, packet.l2);


        return {src_dest->first, src_dest->second, protocol, info};

    }

    return {"","","", info};

}


std::string
PacketRead::format_mac(const u_int8_t *addr) {


    std::ostringstream oss;
    for (int i = 0; i < 6; ++i) {
        if (i > 0)
            oss << ":";
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr[i]);
    }
    return oss.str();


}


std::string PacketRead::format_ipv4_src_dst(const std::byte *addr) {

    std::ostringstream oss;
    for (int i = 0; i < 4; ++i) {
        if (i > 0)
            oss << ":";
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr[i]);
    }

    return oss.str();

}



std::vector<std::byte> PacketRead::copy_packet(const u_int8_t *data, size_t len){

    std::vector<std::byte> packet;
    packet.reserve(len);

    for (int i = 0; i < len; i++) {

        packet.push_back(static_cast<std::byte>(data[i]));

    }

}




