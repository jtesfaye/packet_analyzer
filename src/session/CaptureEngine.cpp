//
// Created by jeremiah tesfaye on 8/18/25.
//

#include <session/CaptureEngine.h>
#include <utility>


CaptureEngine::CaptureEngine(EngineInit &init, std::function<void(std::vector<packet_data>&)> batch_callback)
: on_pkt_batch_callback(std::move(batch_callback))
, m_initial_parser(init.layer2_type, init.flags)
, m_ringbuf(1000, 5)
, m_details_cache(init.cache_size_limit)
, m_sequencer(*this, m_ringbuf, m_data_queue, batch_callback)
, m_raw_pkt_queue(init.queue)
{
    for (size_t i = 0; i < init.thread_count; ++i) {
        m_workers.emplace_back([this] {
            do_work();
        });
    }
    raw_pkt_writer.swap(init.raw_pkt_writer);
    raw_pkt_writer->set_callback(
        std::function{[this] {
            m_work_to_do.notify_all();
        }});
}

void CaptureEngine::do_work() {
    while (true) {
        RawPacket pkt{};
        {
            std::unique_lock u_lock(lock);
            m_work_to_do.wait(u_lock, [this] {
                return m_stop || !m_raw_pkt_queue.empty();
            });

            if (m_stop && m_raw_pkt_queue.empty()) {
                return;
            }
            pkt = m_raw_pkt_queue.pop();
        }
        process_packet(pkt);
    }
}

void CaptureEngine::process_packet(RawPacket pkt) {
    const auto pkt_span = std::span<std::byte>(pkt.packet);
    size_t index = pkt.index;
    packet_data data = m_initial_parser.start_extract(pkt_span, index);
    std::vector<ProtocolDetails> details = m_detail_parser.detail_parse(
        pkt_span,
        data.data);
    m_details_cache.add(index, details);
    data.stream_index = stream_table.add(data);
    m_sequencer.add(std::move(data));
}

void CaptureEngine::shutdown() {
    {
        std::unique_lock u_lock(lock);
        m_stop = true;
    }
    m_sequencer.shutdown();
    m_work_to_do.notify_all();
    for (auto &t : m_workers) {
        if (t.joinable()) {
            t.join();
        }
    }
}

CaptureEngine::PktSequencer::PktSequencer(CaptureEngine& p, PktRingBuffer& b, packet_data_queue& q, const SessionCallback& c)
: engine(p)
, buffer(b)
, seq_queue(q)
, worker([this]{wait_for_next();})
, callback(c)
{}

void CaptureEngine::PktSequencer::add(packet_data && data) {
    {
        std::lock_guard lk(seq_lock);
        seq_queue.push(std::move(data));
    }
    m_cv.notify_all();
}

void CaptureEngine::PktSequencer::wait_for_next() {
    while (true)
    {
        std::unique_lock lk(seq_lock);
        m_cv.wait(lk, [&]
        {
            return !seq_queue.empty() || m_stop || engine.m_stop;
        });
        if ((m_stop || engine.m_stop) && seq_queue.empty()) {
            break;
        }
        if (m_stop || engine.m_stop) {
            while (!seq_queue.empty()) {
                auto pkt = std::move(seq_queue.front());
                seq_queue.pop();
                table[pkt.index] = std::move(pkt);
            }

            for (auto& [_, pkt] : table) {
                buffer.add(std::move(pkt));
            }
            table.clear();

            lk.unlock();

            while (auto view = buffer.get_packet_batch()) {
                callback(view.value());
            }
            return;
        }
        while (!seq_queue.empty()) {
            auto pkt = std::move(seq_queue.front());
            seq_queue.pop();
            table[pkt.index] = std::move(pkt);
        }
        bool ready = table.contains(next_expected);
        lk.unlock();
        if (ready)
        {
            add_to_buffer();
        }
    }
}

void CaptureEngine::PktSequencer::add_to_buffer() {
    std::lock_guard lk(seq_lock);
    size_t start_index = next_expected;
    while (true) {
        if (table.contains(next_expected)) {
            next_expected++;
        } else {
            break;
        }
    }
    if (start_index != next_expected) {
        for (size_t i = start_index; i < next_expected; i++) {
            const auto it = table.find(i);
            buffer.add(std::move(it->second));
            table.erase(it);
        }
        while (true) {
            auto view = buffer.get_packet_batch();
            if (!view) {
                break;
            }
            auto data = std::move(view.value());
            callback(data);
        }
    }
}

void CaptureEngine::PktSequencer::shutdown() {
    {
        std::lock_guard lk(seq_lock);
        m_stop = true;
    }
    m_cv.notify_all();
    if (worker.joinable()) {
        worker.join();
    }
}

CaptureEngine::PktSequencer::~PktSequencer()
{
}

std::shared_ptr<const std::vector<ProtocolDetails>> CaptureEngine::get_detail(size_t pkt_id) {
    return m_details_cache.safe_get(pkt_id);
}

StreamStatistics CaptureEngine::get_stream_stats(size_t pkt_id) {
    return stream_table.get_stats(pkt_id);
}

size_t CaptureEngine::number_of_connections() {
    return stream_table.number_of_connections();
}







