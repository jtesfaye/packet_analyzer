//
// Created by jeremiah tesfaye on 8/15/25.
//

#include <util/PacketObserver.h>
#include <print>

void PacketObserver::notify_if_next(size_t index) {

    std::unique_lock lock(m_lock);

    if (index == m_next_expected)
        m_cv.notify_one();

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

        m_cv.wait(lock, [&] {
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

        emit emit_packets_ready(m_start_index, m_next_expected - 1);
    }
}

