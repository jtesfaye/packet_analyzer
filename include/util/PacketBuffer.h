//
// Created by jeremiah tesfaye on 5/19/25.
//

#ifndef PACKETBUFFER_H
#define PACKETBUFFER_H

#include <vector>
#include <mutex>
#include <QObject>
#include <parsing/PacketParse.h>
#include <parsing/PacketRead.h>

struct layer_details {

    std::optional<link_layer> layer2_details;
    std::optional<net_layer> layer_3_details;

    std::string layer2_str;
};


struct proccessed_packet {

    std::vector<std::string> packet_row_entry;

    layer_details details;

};


class PacketBuffer : public QObject {

    Q_OBJECT

public:

    PacketBuffer() = default;

    ~PacketBuffer() override = default;

    explicit PacketBuffer(QObject* parent = nullptr);

    void add(size_t, packet_ref&);

    [[nodiscard]] proccessed_packet read();

    size_t size();

    signals:

    void new_packet();

private:

    std::vector<packet_ref&> packet_buffer;

    std::atomic<size_t> m_size;

    mutable std::mutex mutex;
};

#endif //PACKETBUFFER_H
