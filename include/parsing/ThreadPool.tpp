//
// Created by jeremiah tesfaye on 8/18/25.
//

#pragma once

inline ThreadPool::ThreadPool(const PoolInit &init)
: m_initial_parser(init.init_parser)
, m_detail_parser(init.detail_parser)
, m_initial_buffer(init.pkt_buffer)
, m_pkt_queue(init.raw_pkt_queue)
, m_details_cache(init.detail_buffer)
{
    for (size_t i = 0; i < init.thread_count; ++i) {
        m_workers.emplace_back([this] {do_work();});
    }
}

inline void ThreadPool::notify_all() {
    m_work_to_do.notify_all();
}

inline void ThreadPool::do_work() {

    while (true) {

        RawPacket pkt{};
        {
            std::unique_lock u_lock(lock);
            m_work_to_do.wait(u_lock, [this] {return m_stop || !m_pkt_queue.empty(); });

            if (m_stop && m_pkt_queue.empty()) return;

            pkt = m_pkt_queue.front();
            m_pkt_queue.pop();

        }

        parse_packet(pkt);

    }
}

inline void ThreadPool::parse_packet(RawPacket pkt) {

    const auto pkt_span = std::span<std::byte>(pkt.packet);

    packet_ref ref = m_initial_parser->start_extract({pkt_span.begin(), pkt_span.end()}, pkt.index);

    std::vector<ProtocolDetails> details = m_detail_parser->detail_parse(
        {pkt_span.begin(), pkt_span.end()},
               ref.data);

    m_initial_buffer->add(pkt.index, std::move(ref));

    m_details_cache->add(pkt.index, details);
}

inline void ThreadPool::shutdown() {

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
