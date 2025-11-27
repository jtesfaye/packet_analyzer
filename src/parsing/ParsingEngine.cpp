//
// Created by jeremiah tesfaye on 8/18/25.
//

#include <parsing/ParsingEngine.h>

ParsingEngine::ParsingEngine(const EngineInit &init)
: m_initial_parser(init.init_parser)
, m_detail_parser(init.detail_parser)
, m_initial_buffer(init.pkt_buffer)
, m_pkt_queue(init.raw_pkt_queue)
, m_details_cache(init.detail_buffer)
, m_observer(init.observer)
, stream_table(init.table)
{
    for (size_t i = 0; i < init.thread_count; ++i) {
        m_workers.emplace_back([this] {do_work();});
    }
}

void ParsingEngine::notify_all() {
    m_work_to_do.notify_all();
}

void ParsingEngine::do_work() {

    while (true) {

        RawPacket pkt{};
        {
            std::unique_lock u_lock(lock);
            m_work_to_do.wait(u_lock, [this] {return m_stop || !m_pkt_queue.empty(); });

            if (m_stop && m_pkt_queue.empty()) return;

            pkt = m_pkt_queue.front();
            m_pkt_queue.pop();

        }
        process_packet(pkt);
    }
}

void ParsingEngine::process_packet(RawPacket pkt) {

    const auto pkt_span = std::span<std::byte>(pkt.packet);
    size_t index = pkt.index;

    packet_ref ref = m_initial_parser->start_extract(pkt_span, index);

    std::vector<ProtocolDetails> details = m_detail_parser->detail_parse(
        pkt_span,
        ref.data);

    m_initial_buffer->add(index, std::move(ref));

    m_details_cache->add(index, details);

   m_observer->notify_if_next(index);
}

void ParsingEngine::shutdown() {

    {
        std::unique_lock u_lock(lock);
        m_stop = true;
    }

    m_work_to_do.notify_all();

    for (auto &t : m_workers) {

        if (t.joinable())
            t.join();

    }
}
