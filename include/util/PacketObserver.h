//
// Created by jeremiah tesfaye on 8/15/25.
//

#ifndef PACKETOBSERVER_H
#define PACKETOBSERVER_H

#include <QObject>
#include <packet/PacketUtil.h>
#include <util/IContainerType.h>
#include <condition_variable>
#include <span>
#include <util/SparsePacketBuffer.h>
#include <util/LRUCache.h>
#include <thread>


class PacketObserver : public QObject {

    Q_OBJECT

public:

    using InitialParseBuffer = SparsePacketBuffer<packet_ref>;
    using DetailParseCache = LRUCache<std::vector<ProtocolDetails>>;

    explicit PacketObserver(InitialParseBuffer& buffer, DetailParseCache& detail_cache)
        : m_buffer(buffer)
        , m_cache(detail_cache) {}

    ~PacketObserver() override;

    void start_observer();

    void notify_if_next(size_t index);

    void notify_all() {m_cv.notify_all();}

    void wait_for_next();

    void set_done() {m_done = true;}

public slots:

    void receive_detail_request(size_t index);

private:

    InitialParseBuffer& m_buffer;
    DetailParseCache& m_cache;

    std::thread m_wait_for_next_worker;
    std::mutex m_lock;
    std::condition_variable m_cv;
    size_t m_next_expected {0};
    size_t m_start_index {0};
    bool m_done {false};

signals:

    void emit_packets_ready(std::deque<packet_ref>::iterator first, std::deque<packet_ref>::iterator last);
    void emit_pkt_details(std::vector<ProtocolDetails>);

};

#endif //PACKETOBSERVER_H
