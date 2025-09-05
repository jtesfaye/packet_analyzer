//
// Created by jeremiah tesfaye on 8/31/25.
//

#include <layer3_protocols/IPv4.h>
#include <sstream>


std::string
IPv4_functions::ipv4_src_dest_format(const std::byte *addr) {

    std::ostringstream oss;
    for (int i = 0; i < 4; ++i) {
        if (i > 0)
            oss << ".";
        oss << std::hex << static_cast<int>(addr[i]);
    }

    return oss.str();

}


net_layer_ref
IPv4_functions::IPv4_parse(const std::vector<std::byte> &raw_data, parse_context &context) {

    using namespace packet::ip;

    if (raw_data.size() < sizeof(ipv4_header)) {
        throw std::runtime_error("Packet is too small to contain ipv4 header");
    }

    const std::byte* start = raw_data.data() + context.offset;

    const auto ipv4_hdr = reinterpret_cast<const ipv4_header*> (start);

    row_entry& entry = context.entry;

    context.length = (ipv4_hdr->version_ihl & 0x0F) * 4;
    context.next_type = ipv4_hdr->protocol;

    const auto* src_start = reinterpret_cast<const std::byte*>(&ipv4_hdr->src_addr);
    const auto* dest_start = reinterpret_cast<const std::byte*> (&ipv4_hdr->dest_adr);

    entry.src = ipv4_src_dest_format(src_start);
    entry.dest = ipv4_src_dest_format(dest_start);

    return net_layer_ref{static_cast<ipv4_header*>(nullptr)};

    //ihl tells us if its full length
    //need to check if its fragmented

}

