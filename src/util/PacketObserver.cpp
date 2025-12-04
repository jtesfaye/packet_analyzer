//
// Created by jeremiah tesfaye on 8/15/25.
//

#include <util/PacketObserver.h>
#include <print>
#include <QtConcurrent>

#include "model/RowModel.h"

void PacketObserver::notify_if_next(size_t index) {

    std::unique_lock lock(m_lock);

    if (index == m_next_expected)
        m_cv.notify_one();

}

void PacketObserver::start_observer() {

    m_wait_for_next_worker = std::thread([this] () {
        wait_for_next();
    });

    m_wait_for_next_worker.detach();

}


/*
 * wait_for_next()
 *
 * Continuously monitors the packet buffer for the next expected packet index.
 * When one or more consecutive packets are available, it emits the signal
 * `emit_packets_ready` with the inclusive range of indices that are ready.
 *
 * Behavior:
 * - Waits until the next expected packet is present in the buffer, or until
 *   the observer is marked as done.
 * - Determines the contiguous sequence of available packets starting from
 *   `m_next_expected`.
 * - Updates `m_next_expected` to reflect the next packet to wait for.
 * - Emits `emit_packets_ready(start_index, end_index)` for the contiguous batch.
 *
 * This function runs in a loop, allowing it to continuously provide batches
 * of ready packets to connected slots.
 */
void PacketObserver::wait_for_next() {

    while (true) {

        std::unique_lock lock(m_lock);

        m_cv.wait(lock, [&]() {
            return m_buffer.exists(m_next_expected) || m_done;
        });

        if (m_done && !m_buffer.exists(m_next_expected)) {
            return;
        }

        m_start_index = m_next_expected;

        while (true) {

            if (m_buffer.exists(m_next_expected)) {
                m_next_expected++;
            } else {
                break;
            }
        }

        emit emit_packets_ready(m_buffer.begin_at(m_start_index), m_buffer.begin_at(m_next_expected));
    }
}

void PacketObserver::receive_detail_request(size_t index) {

    if (!m_cache.exists(index)) {
        emit_pkt_details({{"N/A", {}}});
    }

    const std::vector<ProtocolDetails> vec = *m_cache.safe_get(index);
    emit_pkt_details(vec);

}

void PacketObserver::recieve_stream_stat_request(size_t index) {

    std::shared_ptr<Stream> stream;
    stream_table.get_stream(index, stream);
    std::shared_ptr<StreamStats> stats = stream->get_stats();
    emit emit_stream_stat(stats);

}

PacketObserver::~PacketObserver() {

    set_done();
    m_cv.notify_all();

    if (m_wait_for_next_worker.joinable()) {
        m_wait_for_next_worker.join();
    }
}

void PacketObserver::init_observer(Models &models) {
    connect_to_table(models.row_model);
    connect_to_detail_pane(models.detail_model);
}

void PacketObserver::connect_to_table(RowModel* model) {
    connect(this, &PacketObserver::emit_packets_ready, model, &RowModel::add_data, Qt::QueuedConnection);
}

void PacketObserver::connect_to_detail_pane(DetailModel *model) {
    connect(this, &PacketObserver::emit_pkt_details, model, &DetailModel::set_data, Qt::QueuedConnection);
}

void PacketObserver::connect_to_throughput_chart(ThroughputChart* chart) {
    connect(this, &PacketObserver::emit_stream_stat, chart, &ThroughputChart::addValue);
}



