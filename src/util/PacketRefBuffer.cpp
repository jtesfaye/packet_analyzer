//
// Created by jeremiah tesfaye on 8/14/25.
//

#include <util/PacketRefBuffer.h>

PacketRefBuffer::PacketRefBuffer(size_t capacity)
: m_capacity(capacity) {

    buffer.resize(m_capacity);
}


void PacketRefBuffer::add(size_t index, packet_ref&& ref) {

    if (index > buffer.size() - 1) {
        m_capacity *= 2;
        buffer.resize(m_capacity);
    }

    buffer[index] = std::move(ref);

}

packet_ref &PacketRefBuffer::get_ref(const size_t index) {

    if (index > buffer.size() - 1)
        throw std::runtime_error("PacketRefBuffer get_ref(): Access out of bounds on index " + std::to_string(index) + "\n");

    if (buffer[index] == std::nullopt) {
        throw std::runtime_error("PacketRefBuffer get_ref(): nullopt");
    }

    return buffer[index].value();
}

bool PacketRefBuffer::exists(size_t index) const {

    if (index > buffer.size() - 1) {
        return false;
    }

    if (buffer[index] == std::nullopt)
        return false;

    return true;

}



