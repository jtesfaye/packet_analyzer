//
// Created by jeremiah tesfaye on 7/27/25.
//

#include <iostream>
#include <layerx/layer2/Ethernet.h>
#include <layerx/layer2/Layer2Registry.h>

Ethernet::Ethernet(const size_t len, const u_int8_t *src, const u_int8_t *dest, const u_int16_t ether_type)
: LinkPDU(len)
, ether_type(ether_type)
{
    std::memcpy(&src_address.bytes, src, ethernet::addr_len);
    std::memcpy(&dest_address.bytes, dest, ethernet::addr_len);

    src_address.size = ethernet::addr_len;
    dest_address.size = ethernet::addr_len;
}

Ethernet::~Ethernet() = default;

std::string Ethernet::make_info() const {

    std::string protocol;

    switch (ether_type) {
        case 0x0800: protocol = "IPv4"; break;
        case 0x0806: protocol = "ARP"; break;
        case 0x86DD: protocol = "IPv6"; break;
        default: protocol = std::format("0x{:04X}", ether_type);
    }

    return std::format("EtherType= {}" ,  protocol);
}

std::string_view Ethernet::name() const {
    static const std::string name = "Ethernet II";
    return name;
}

std::string Ethernet::address_to_string(const Address &addr) const {
    return format_mac(reinterpret_cast<const u_int8_t*>(addr.bytes.data()));
}

Address Ethernet::src() const {
    return src_address;
}

Address Ethernet::dest() const {
    return dest_address;
}

void ethernet::register_ethernet() {
    registry::layer2::register_self(static_cast<int>(ProtocolKeys::ETH), ethernet_parse);
    registry::layer2::register_self(static_cast<int>(ProtocolKeys::ETH), ethernet_detailed_parse);
}

std::unique_ptr<LinkPDU> ethernet::ethernet_parse(
    std::span<std::byte> raw_data,
    parse_context& context) {

    size_t start = context.offset;
    if(!valid_length(raw_data, start, sizeof(ethernet_hdr))) {
        return nullptr;
    }

    const std::byte* layer2_start = raw_data.data() + start;

    const auto ether = reinterpret_cast<const ethernet_hdr*> (layer2_start);

    u_int16_t ether_type = ntohs(ether->ether_type);

    switch (ether_type) {

        case 0x8100: {

            if (!valid_length(raw_data, start, sizeof(ether_802_1_Q_hdr)))
                return nullptr;

            const auto vlan = reinterpret_cast<const ether_802_1_Q_hdr*> (layer2_start);
            ether_type = ntohs(vlan->ether_type);
            context.curr_length = 18;
            break;
        }

        case 0x88A8: {

            if (!valid_length(raw_data, start, sizeof(ether_802_1_ad_hdr)))
                return nullptr;

            const auto q_ad = reinterpret_cast<const ether_802_1_ad_hdr*> (layer2_start);
            ether_type = ntohs(q_ad->ether_type);
            context.curr_length = 22;
            break;
        }

        default: {
            context.curr_length = 14;
            break;
        }
    }

    context.next_type = ether_type;

    return std::make_unique<Ethernet>(
        context.curr_length,
        ether->src_addr,
        ether->dest_addr,
        ether_type
        );
}

ProtocolDetails ethernet::ethernet_detailed_parse(
    std::span<std::byte> raw_data,
    parse_context &context) {

    std::vector<std::string> details;

    auto hdr = reinterpret_cast<const ethernet_hdr*> (raw_data.data() + context.offset);
    u_int16_t ether_type = hdr->ether_type;

    switch (ether_type) {

        case 0x8100: {

            auto vlan = reinterpret_cast<const ether_802_1_Q_hdr*> (raw_data.data() + context.offset);
            uint16_t tci = ntohs(vlan->tci);
            uint16_t vlan_id = tci & 0x0FFF;
            uint8_t pcp = (tci >> 13) & 0x07;
            bool dei = (tci >> 12) & 0x01;
            details.push_back(std::format("802.1Q VLAN Tag Control Information: 0x{:04X}", tci));
            details.push_back(std::format("  Priority Code Point (PCP): {}", pcp));
            details.push_back(std::format("  Drop Eligible Indicator (DEI): {}", dei));
            details.push_back(std::format("  VLAN ID: {}", vlan_id));
            details.push_back(std::format("Encapsulated EtherType: 0x{:04X}", ntohs(vlan->ether_type)));
            break;

        }

        case 0x88A8: {

            auto ad = reinterpret_cast<const ether_802_1_ad_hdr*> (raw_data.data() + context.offset);
            uint16_t outer_tpid = ntohs(ad->tpid);
            uint16_t outer_tci  = ntohs(ad->tci);
            uint16_t inner_tpid = ntohs(ad->tpid_2);
            uint16_t inner_tci  = ntohs(ad->tci_2);
            uint16_t ether_type = ntohs(ad->ether_type);

            uint8_t outer_pcp = (outer_tci >> 13) & 0x07;
            bool outer_dei = (outer_tci >> 12) & 0x01;
            uint16_t outer_vlan_id = outer_tci & 0x0FFF;

            uint8_t inner_pcp = (inner_tci >> 13) & 0x07;
            bool inner_dei = (inner_tci >> 12) & 0x01;
            uint16_t inner_vlan_id = inner_tci & 0x0FFF;

            details.push_back(std::format("802.1ad (Q-in-Q) VLAN Tagging"));
            details.push_back(std::format("  Outer TPID: 0x{:04X}", outer_tpid));
            details.push_back(std::format("  Outer VLAN TCI: 0x{:04X}", outer_tci));
            details.push_back(std::format("    Priority Code Point (PCP): {}", outer_pcp));
            details.push_back(std::format("    Drop Eligible Indicator (DEI): {}", outer_dei));
            details.push_back(std::format("    VLAN ID: {}", outer_vlan_id));

            details.push_back(std::format("  Inner TPID: 0x{:04X}", inner_tpid));
            details.push_back(std::format("  Inner VLAN TCI: 0x{:04X}", inner_tci));
            details.push_back(std::format("    Priority Code Point (PCP): {}", inner_pcp));
            details.push_back(std::format("    Drop Eligible Indicator (DEI): {}", inner_dei));
            details.push_back(std::format("    VLAN ID: {}", inner_vlan_id));

            details.push_back(std::format("Encapsulated EtherType: 0x{:04X}", ether_type));
            break;

        }

        default: {

            std::string src = format_mac(hdr->src_addr);
            std::string dst = format_mac(hdr->dest_addr);

            details.push_back(std::format("Destination MAC: {}", dst));
            details.push_back(std::format("Source MAC: {}", src));
            details.push_back(std::format("EtherType: 0x{:04X}", ether_type));
            break;

        }
    }

    return {full_protocol_name, details};
}


