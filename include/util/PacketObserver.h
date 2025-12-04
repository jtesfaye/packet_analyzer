//
// Created by jeremiah tesfaye on 8/15/25.
//

#ifndef PACKETOBSERVER_H
#define PACKETOBSERVER_H

#include <span>
#include <thread>
#include <QObject>
#include <condition_variable>

#include <capture/CaptureConfig.h>
#include <packet/PacketUtil.h>
#include <util/LRUCache.h>
#include <util/IContainerType.h>
#include <util/SparsePacketBuffer.h>
#include <session/StreamTable.h>
#include <model/RowModel.h>
#include <model/DetailModel.h>
#include <model/Models.h>

struct ObserverInit {
    SparsePacketBuffer<packet_ref>& buffer;
    LRUCache<std::vector<ProtocolDetails>>& detail_cache;
    StreamTable& stream_table;
};

class PacketObserver : public QObject {

    Q_OBJECT

public:

    using InitialParseBuffer = SparsePacketBuffer<packet_ref>;
    using DetailParseCache = LRUCache<std::vector<ProtocolDetails>>;

    explicit PacketObserver(const ObserverInit& init)
        : m_buffer(init.buffer)
        , m_cache(init.detail_cache)
        , stream_table(init.stream_table) {}

    ~PacketObserver() override;

    void start_observer();

    void notify_if_next(size_t index);

    void notify_all() {m_cv.notify_all();}

    void wait_for_next();

    void set_done() {m_done = true;}
    void init_observer(Models& models);
public slots:

    void receive_detail_request(size_t index);
    void recieve_stream_stat_request(size_t index);
    void recieve_global_stat_request();

private:

    void connect_to_table(RowModel* model);
    void connect_to_detail_pane(DetailModel* model);
    void connect_to_throughput_chart(ThroughputChart* chart);

    InitialParseBuffer& m_buffer;
    DetailParseCache& m_cache;
    StreamTable& stream_table;

    std::thread m_wait_for_next_worker;
    std::mutex m_lock;
    std::condition_variable m_cv;
    size_t m_next_expected {0};
    size_t m_start_index {0};
    bool m_done {false};

signals:

    void emit_packets_ready(std::deque<packet_ref>::iterator first, std::deque<packet_ref>::iterator last);
    void emit_pkt_details(std::vector<ProtocolDetails>);
    void emit_stream_stat(std::shared_ptr<StreamStats> stats);

};

#endif //PACKETOBSERVER_H
