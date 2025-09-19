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
        std::print("Indicies that are ready: {} -> {}", m_start_index, m_next_expected - 1);
        //emit_packets_ready(m_start_index, m_next_expected - 1);
    }
}

