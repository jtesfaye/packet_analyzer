//
// Created by jeremiah tesfaye on 8/14/25.
//

#include <util/PacketRefBuffer.h>

void PacketRefBuffer::add(size_t index, packet_tuple&& tup) {

    std::lock_guard lock(m_lock);

    if (index > buffer.size() - 1) {
        m_capacity *= 2;
        buffer.resize(m_capacity);
    }

    buffer[index] = std::move(tup);

}

packet_ref &PacketRefBuffer::get_ref(const size_t index) {

    if (index > buffer.size() - 1)
        throw std::runtime_error("PacketRefBuffer get_ref(): Access out of bounds\n");

    if (buffer[index] == std::nullopt) {
        throw std::runtime_error("PacketRefBuffer get_ref(): nullopt");
    }

    return buffer[index]->second;
}

row_entry &PacketRefBuffer::get_entry(size_t index) {

    if (index > buffer.size() - 1)
        throw std::runtime_error("PacketRefBuffer get(): Access out of bounds\n");

    return buffer[index]->first;

}

bool PacketRefBuffer::exists(size_t index) const {

    if (index > buffer.size() - 1) {
        throw std::runtime_error("PacketRefBuffer exists(): Access out of bounds\n");
    }

    if (buffer[index] == std::nullopt)
        return false;

    return true;

}



