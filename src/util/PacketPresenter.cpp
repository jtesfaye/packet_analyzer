//
// Created by jeremiah tesfaye on 8/20/25.
//

#include <util/PacketPresenter.h>

//notifed when packets are ready
//transforms a series of packets
//send to controller in formateed fashion

PacketPresenter::PacketPresenter(PacketObserver& observer, PcapArray& packets)
: m_observer(observer)
, m_packets(packets)
, m_thread(std::thread(&PacketPresenter::loop, this))
, m_stop(false)
{}

void PacketPresenter::stop() {

    {
        std::unique_lock u_lock(m_mutex);
        m_stop = true;

    }

    m_cv.notify_all();
    m_thread.join();

}

void PacketPresenter::process_packet(size_t start, size_t end) {

    m_tasks.emplace(start,end);
    m_cv.notify_one();

}


void PacketPresenter::loop() {

    std::unique_lock u_lock(m_mutex);

    while (!m_stop) {

        m_cv.wait(u_lock, [&] { return m_stop || !m_tasks.empty(); });

        if (m_stop) break;

        while (!m_tasks.empty()) {

            //go through process of transforming packet then send to controller

        }


    }


}



