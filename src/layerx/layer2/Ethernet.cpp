//
// Created by jeremiah tesfaye on 7/27/25.
//

#include <layerx/layer2/Ethernet.h>
#include <util/PacketRead.h>

Ethernet::Ethernet(size_t len, std::string src, std::string dest, u_int16_t ether_type)
: LinkPDU(len, std::move(src), std::move(dest))
, ether_type(ether_type)
{
}

std::string Ethernet::make_info() const {

    std::string protocol;

    switch (ether_type) {
        case 0x0800: protocol = "IPv4"; break;
        case 0x0806: protocol = "ARP"; break;
        case 0x86DD: protocol = "IPv6"; break;
        default: protocol = std::format("0x{:04X}", ether_type);
    }

    return std::format("{} -> {} EtherType= {}" , src, dest, protocol);

}

std::string Ethernet::name() const {

    return {"Ethernet II"};

}

std::unique_ptr<LinkPDU>
ethernet_functions::ethernet_parse
(const std::vector<std::byte> &raw_data, parse_context& context) {

    using namespace packet::frame;
    using namespace packet;

    if (raw_data.size() < sizeof(ethernet_hdr)) {
        throw std::runtime_error("Packet too short for ethernet header");
    }

    const std::byte* layer2_start = raw_data.data() + context.offset;

    auto ether = reinterpret_cast<const ethernet_hdr*> (layer2_start);

    u_int16_t ether_type = ntohs(ether->ether_type);

    switch (ether_type) {

        case 0x8100:

            auto vlan = reinterpret_cast<const ether_802_1_Q_hdr*> (layer2_start);

            ether_type = ntohs(vlan->ether_type);

            context.length = 18;

            break;

        case 0x88A8:

            auto q_ad = reinterpret_cast<const ether_802_1_ad_hdr*> (layer2_start);

            ether_type = ntohs(q_ad->ether_type);

            context.length = 22;
            break;

        default:

            context.length = 14;
    }

    context.next_type = ether_type;

    std::string src = PacketRead::format_mac(ether->src_addr);
    std::string dest = PacketRead::format_mac(ether->dest_addr);

    return std::make_unique<Ethernet>(
        context.length,
        std::move(src),
        std::move(dest),
        ether_type
        );
}

