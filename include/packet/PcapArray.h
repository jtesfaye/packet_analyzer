//
// Created by jeremiah tesfaye on 7/30/25.
//

#ifndef PCAPARRAY_H
#define PCAPARRAY_H

#include <vector>
#include <pcap/pcap.h>
#include <packet/PcapReader.h>
#include <optional>

/*
 * Purpose:
 * Array stores metadata for packet captures,
 * goal is to make it easy to find offset and length for each packet.
 */
class PcapArray {
public:
    struct pkt_metadata;

    PcapArray() = delete;

    explicit PcapArray(size_t file_size, size_t glb_hdr_size);

    explicit PcapArray(std::unique_ptr<PcapReader> reader, size_t file_size, size_t glb_hdr_size);

    pkt_metadata get(size_t index) const;

    int add_packet_index(size_t pkt_size);

    size_t size() const {return m_packets.size();}

    struct pkt_metadata {

        size_t offset;
        size_t pkt_size;

    };

private:

    void build_index();

    std::unique_ptr<PcapReader> m_reader;

    size_t m_file_size;

    size_t m_total_packets;

    size_t m_bytes_before_insert;

    std::vector<pkt_metadata> m_packets;

};

#endif //PCAPARRAY_H
