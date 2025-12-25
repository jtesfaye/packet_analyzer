//
// Created by jeremiah tesfaye on 8/15/25.
//

#ifndef PACKETOBSERVER_H
#define PACKETOBSERVER_H

#include <span>
#include <thread>
#include <condition_variable>
#include <capture/CaptureConfig.h>
#include <util/PacketUtil.h>
#include <util/LRUCache.h>
#include <util/IContainerType.h>
#include <util/SparsePacketBuffer.h>
#include <stream/StreamTable.h>
#include <interfaces/IEventSink.h>

struct ObserverInit {
    SparsePacketBuffer<packet_ref>& buffer;
    LRUCache<std::vector<ProtocolDetails>>& detail_cache;
    StreamTable& stream_table;
    IEventSink& sink;
};

class PacketObserver {
public:

    using InitialParseBuffer = SparsePacketBuffer<packet_ref>;
    using DetailParseCache = LRUCache<std::vector<ProtocolDetails>>;

    explicit PacketObserver(const ObserverInit& init)
        : m_buffer(init.buffer)
        , m_cache(init.detail_cache)
        , stream_table(init.stream_table)
        , sink(init.sink) {

        m_wait_for_next_worker = std::thread([this] () {
            wait_for_next();
        });
        m_wait_for_next_worker.detach();
    }
    ~PacketObserver();

    void wait_for_next();
    void notify_if_next(size_t index);
    void notify_all() {
        m_cv.notify_all();
    }
    void set_done() {
        m_done = true;
    }

private:

    InitialParseBuffer& m_buffer;
    DetailParseCache& m_cache;
    StreamTable& stream_table;
    IEventSink& sink;

    std::thread m_wait_for_next_worker;
    std::mutex m_lock;
    std::condition_variable m_cv;
    size_t m_next_expected {0};
    size_t m_start_index {0};
    bool m_done {false};
};

#endif //PACKETOBSERVER_H
