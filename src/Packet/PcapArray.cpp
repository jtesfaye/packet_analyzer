//
// Created by jeremiah tesfaye on 7/30/25.
//

#include "../../include/packet/PcapArray.h"


int
PcapArray::add_packet_index(size_t hdr_size, size_t pkt_size) {

    lock.lock();

    size_t offset = m_bytes_before_insert + 1; //start of new packet with header

    packets.emplace_back(PacketIndex{offset, pkt_size, hdr_size});

    m_bytes_before_insert += hdr_size + pkt_size;

    lock.unlock();

    return 0;

}

const u_int8_t
*PcapArray::operator[](size_t index) {

    if (index >= packets.size()) {
        return nullptr;
    }

    size_t offset = packets[index].offset;
    size_t pkt_size = packets[index].pkt_size;

}