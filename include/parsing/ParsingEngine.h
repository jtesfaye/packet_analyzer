//
// Created by jeremiah tesfaye on 8/18/25.
//

#ifndef PARSINGENGINE_H
#define PARSINGENGINE_H

#include <thread>
#include <vector>
#include <parsing/InitialParser.h>
#include <parsing/DetailParser.h>
#include <util/IContainerType.h>
#include <util/PacketObserver.h>
#include <atomic>
#include <boost/lockfree/spsc_queue.hpp>
#include <session/StreamTable.h>

using namespace boost::lockfree;
using raw_pkt_queue = spsc_queue<RawPacket, capacity<255>> ;

struct EngineInit {
    const std::shared_ptr<InitialParser>init_parser;
    const std::shared_ptr<DetailParser> detail_parser;
    const std::shared_ptr<IContainerType<packet_ref>> pkt_buffer;
    const std::shared_ptr<IContainerType<std::vector<ProtocolDetails>>> detail_buffer;
    const std::shared_ptr<PacketObserver> observer;
    raw_pkt_queue& raw_pkt_queue;
    size_t thread_count = std::thread::hardware_concurrency();
    StreamTable& table;
};

class ParsingEngine {
public:

    explicit ParsingEngine(const EngineInit &init);

    ~ParsingEngine() = default;

    void do_work();

    void shutdown();

    void notify_all();

private:

    void process_packet(RawPacket& pkt);

    std::shared_ptr<InitialParser> m_initial_parser;
    std::shared_ptr<DetailParser> m_detail_parser;

    std::shared_ptr<IContainerType<packet_ref>> m_initial_buffer;
    std::shared_ptr<IContainerType<std::vector<ProtocolDetails>>> m_details_cache;
    std::shared_ptr<PacketObserver> m_observer;
    raw_pkt_queue& m_pkt_queue;

    StreamTable& stream_table;

    std::vector<std::thread> m_workers;
    std::mutex lock;
    std::condition_variable m_work_to_do;
    std::atomic<bool> m_stop{false};

};

#endif //PARSINGENGINE_H
