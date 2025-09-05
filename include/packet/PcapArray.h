//
// Created by jeremiah tesfaye on 7/30/25.
//

#ifndef PCAPARRAY_H
#define PCAPARRAY_H

#include <vector>
#include <pcap/pcap.h>

#include "PcapFile.h"
/* What we need:
 * Access to memory map of pcap file
 */
class PcapArray {
public:

    explicit PcapArray(PcapFile& file)
    : file(file)
    , m_total_packets(0)
    , m_bytes_before_insert(PcapFile::m_global_header_size) {}

    const u_int8_t* operator[](size_t index);

    int add_packet_index(size_t hdr_size, size_t pkt_size);

private:

    std::mutex lock;

    PcapFile& file;

    size_t m_total_packets;

    size_t m_bytes_before_insert;

    struct PacketIndex {

        size_t offset;
        size_t pkt_size;
        size_t header_size;

    };

    std::vector<PacketIndex> packets;


};



#endif //PCAPARRAY_H
