//
// Created by jeremiah tesfaye on 8/15/25.
//

#ifndef PACKETOBSERVER_H
#define PACKETOBSERVER_H

#include <QObject>
#include <util/PacketRefBuffer.h>
#include <condition_variable>

class PacketObserver : public QObject {
public:

    explicit PacketObserver(PacketRefBuffer& buffer)
        : m_buffer(buffer) {}

    ~PacketObserver() override = default;

    void notify_if_next(size_t index);

    void notify_all() {m_cv.notify_all();}

    void wait_for_next();

    void set_done() {m_done = true;}

private:

    PacketRefBuffer& m_buffer;
    std::mutex m_lock;
    std::condition_variable m_cv;
    size_t m_next_expected {0};
    size_t m_start_index {0};
    bool m_done {false};

signals:

    void emit_packets_ready(size_t start, size_t end);

};

#endif //PACKETOBSERVER_H
