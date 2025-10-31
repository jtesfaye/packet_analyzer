//
// Created by jeremiah tesfaye on 8/18/25.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <vector>
#include <parsing/InitialParser.h>
#include <parsing/DetailParser.h>
#include <util/IContainerType.h>
#include <util/LRUCache.h>
#include <atomic>
#include <queue>
#include <boost/lockfree/spsc_queue.hpp>

using namespace boost::lockfree;
using raw_pkt_queue = spsc_queue<RawPacket, capacity<255>> ;

struct PoolInit {
    const std::shared_ptr<InitialParser>init_parser;
    const std::shared_ptr<DetailParser> detail_parser;
    const std::shared_ptr<IContainerType<packet_ref>> pkt_buffer;
    const std::shared_ptr<IContainerType<std::vector<ProtocolDetails>>> detail_buffer;
    raw_pkt_queue& raw_pkt_queue;
    size_t thread_count = std::thread::hardware_concurrency();
};

class ThreadPool {
public:

    explicit ThreadPool(const PoolInit &init);

    ~ThreadPool() = default;

    void do_work();

    void shutdown();

    void notify_all();

private:

    void parse_packet(RawPacket pkt);

    std::shared_ptr<InitialParser> m_initial_parser;
    std::shared_ptr<DetailParser> m_detail_parser;

    std::shared_ptr<IContainerType<packet_ref>> m_initial_buffer;
    std::shared_ptr<IContainerType<std::vector<ProtocolDetails>>> m_details_cache;
    raw_pkt_queue& m_pkt_queue;

    std::vector<std::thread> m_workers;
    std::mutex lock;
    std::condition_variable m_work_to_do;
    std::atomic<bool> m_stop{false};

};

#include "ThreadPool.tpp"


#endif //THREADPOOL_H
