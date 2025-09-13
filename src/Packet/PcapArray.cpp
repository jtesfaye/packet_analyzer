//
// Created by jeremiah tesfaye on 7/30/25.
//

#include <print>
#include <packet/PacketUtil.h>
#include <packet/PcapArray.h>

PcapArray::PcapArray( size_t file_size, size_t glbhdr_size)
: m_file_size(file_size)
, m_total_packets(0)
, m_bytes_before_insert(glbhdr_size)
,m_reader(nullptr)
{
}

PcapArray::PcapArray(std::unique_ptr<PcapReader> reader, size_t file_size, size_t glb_hdr)
: m_reader(std::move(reader))
, m_file_size(file_size)
, m_bytes_before_insert(glb_hdr)
{

    build_index();
    m_total_packets = m_packets.size();

}


int
PcapArray::add_packet_index(const size_t pkt_size) {

    const size_t offset = m_bytes_before_insert; //start of new packet with header

    m_packets.emplace_back(pkt_metadata{offset, pkt_size});

    std::println("Current size {}", m_packets.size());

    m_bytes_before_insert += sizeof(packet::pcaprec_hdr_t) + pkt_size;

    return 0;

}


/**
 * Used to find the starting position of each packet for the "packets" vector if user is reading
 * from a pre-existing .pcap file
 */
void
PcapArray::build_index() {

    size_t glb_header = m_bytes_before_insert;
    u_int64_t offset = glb_header;
    u_int64_t file_size = m_file_size;
    size_t pkt_header_size = 16;

    PcapReader& reader = *m_reader;

    while (offset + pkt_header_size <= file_size) {

        std::vector<std::byte> byte_arr = reader.read(offset, pkt_header_size);

        const auto* header = reinterpret_cast<const packet::pcaprec_hdr_t*>(byte_arr.data());

        if (!header) {
            break;
        }

        if (header->incl_len == 0) {
            break;
        }

        u_int32_t pkt_size = header->incl_len;

        m_packets.emplace_back(pkt_metadata{offset,pkt_header_size + pkt_size});
        offset += pkt_header_size + pkt_size;
    }

    m_bytes_before_insert = offset;

}

PcapArray::pkt_metadata
PcapArray::get(const size_t index) const {

    if (index >= m_packets.size()) {
        throw std::out_of_range("PacketArray: Index is out of bounds");
    }

    return m_packets[index];

}
