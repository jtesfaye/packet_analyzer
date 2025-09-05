//
// Created by jeremiah tesfaye on 8/14/25.
//

#ifndef PACKETREFBUFFER_H
#define PACKETREFBUFFER_H

#include <QObject>
#include <packet/LayerWrappers.h>

using namespace packet;

class PacketRefBuffer : public QObject {

Q_OBJECT

public:

    using packet_tuple = std::pair<row_entry, packet_ref>;

    explicit PacketRefBuffer(size_t capacity, QObject* parent = nullptr)
    : m_capacity(capacity) {

        if (capacity < 1) {
            m_capacity(256);
        }
        buffer.resize(m_capacity, std::nullopt);
    }

    void add(size_t index, packet_tuple&&);

    packet_ref& get_ref(size_t index);

    row_entry& get_entry(size_t index);

    [[nodiscard]] bool exists(size_t index) const;

private:

    size_t m_capacity;

    std::vector<std::optional<packet_tuple>> buffer;
    std::mutex m_lock;

};


#endif //PACKETREFBUFFER_H
