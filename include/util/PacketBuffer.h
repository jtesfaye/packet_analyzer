//
// Created by jeremiah tesfaye on 8/14/25.
//

#ifndef PACKETREFBUFFER_H
#define PACKETREFBUFFER_H

#include <deque>
#include <packet/PacketUtil.h>
#include <optional>
#include <deque>

using namespace packet;

class PacketRefBuffer {

public:

    explicit PacketRefBuffer(size_t capacity);
    ~PacketRefBuffer() = default;

    void add(size_t index, packet_ref&&);

    packet_ref& get_ref(size_t index);

    [[nodiscard]] bool exists(size_t index) const;

    [[nodiscard]] size_t size() const {return buffer.size();}

private:

    size_t m_capacity;
    std::deque<std::optional<packet_ref>> buffer;
    std::mutex m_lock;

};

#endif //PACKETREFBUFFER_H
