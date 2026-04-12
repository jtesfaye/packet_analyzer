//
// Created by jeremiah tesfaye on 8/18/25.
//

#ifndef CAPTUREENGINE_H
#define CAPTUREENGINE_H

#include <thread>
#include <future>
#include <vector>
#include <atomic>
#include <queue>
#include <capture/PacketCapture.h>
#include <parsing/InitialParser.h>
#include <parsing/DetailParser.h>
#include <util/PktRingBuffer.h>
#include <util/LRUCache.h>
#include <stream/StreamTable.h>

using packet_data_queue = std::queue<packet_data>;
using SessionCallback = std::function<void(std::vector<packet_data>&)>;
struct EngineInit {

    EngineInit(
        std::unique_ptr<PacketCapture> raw_pkt_writer_,
        int layer2_type_,
        u_int8_t flags_,
        size_t cache_size_limit_,
        raw_packet_queue& queue_,
        size_t thread_count_ = std::thread::hardware_concurrency()
    )
        : raw_pkt_writer(std::move(raw_pkt_writer_)),
          layer2_type(layer2_type_),
          flags(flags_),
          cache_size_limit(cache_size_limit_),
          thread_count(thread_count_),
          queue(queue_) {}

    std::unique_ptr<PacketCapture> raw_pkt_writer;
    int layer2_type;
    u_int8_t flags;
    size_t cache_size_limit;
    size_t thread_count = std::thread::hardware_concurrency();
    raw_packet_queue& queue;
};

class CaptureEngine {
public:

    explicit CaptureEngine(
        EngineInit &init,
        SessionCallback callback
        );

    ~CaptureEngine()
    {
        shutdown();
    }

    void start() {
        if (raw_pkt_writer) {
            auto task = [this] {
                raw_pkt_writer->start_capture();
            };
            comp.push_back(std::async(std::launch::async, task));
        }
    }

    void stop() {
        if (raw_pkt_writer)
            raw_pkt_writer->stop_capture();
    }

    void shutdown();

    std::shared_ptr<const std::vector<ProtocolDetails>> get_detail(size_t pkt_id);
    StreamStatistics get_stream_stats(size_t pkt_id);
    size_t number_of_connections();

    friend class PktSequencer;

    packet_data_queue& get_data_pkt_queue() {
        return m_data_queue;
    }

private:

    void do_work();

    void process_packet(RawPacket pkt);

    class PktSequencer {
    public:
        const size_t wake_up_interval = 10;

        PktSequencer(CaptureEngine& p, PktRingBuffer& b, packet_data_queue& q, const SessionCallback& c);
        ~PktSequencer();

        void add(packet_data&&);
        void on_packet_batch();
        void shutdown();

    private:
        void wait_for_next();
        void add_to_buffer();

        SessionCallback callback;
        size_t next_expected{};
        CaptureEngine& engine;
        PktRingBuffer& buffer;
        packet_data_queue& seq_queue;
        std::unordered_map<size_t, packet_data> table;
        std::thread worker;
        std::condition_variable m_cv;
        std::mutex seq_lock;
        bool m_stop{false};
    };

    raw_packet_queue& m_raw_pkt_queue;
    std::vector<std::future<void>> comp;
    std::function<void(std::vector<packet_data>&)> on_pkt_batch_callback;
    std::unique_ptr<PacketCapture> raw_pkt_writer;
    InitialParser m_initial_parser;
    DetailParser m_detail_parser;
    PktRingBuffer m_ringbuf;
    LRUCache<std::vector<ProtocolDetails>> m_details_cache;
    StreamTable stream_table;
    packet_data_queue m_data_queue;
    std::vector<std::thread> m_workers;
    std::mutex lock;
    std::condition_variable m_work_to_do;
    std::atomic<bool> m_stop{false};
    PktSequencer m_sequencer;
};

#endif //CAPTUREENGINE_H
