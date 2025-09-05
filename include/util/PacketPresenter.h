//
// Created by jeremiah tesfaye on 8/20/25.
//

#include <QObject>
#include <util/PacketObserver.h>
#include <packet/PcapArray.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <queue>

#ifndef PACKETPRESENTER_H
#define PACKETPRESENTER_H

class PacketPresenter : public QObject {

public:

    explicit PacketPresenter(PacketObserver&, PcapArray&);
    ~PacketPresenter() override;

    void stop();

    void loop();

public slots:

    void process_packet(size_t start, size_t end);

    PacketObserver& m_observer;
    PcapArray& m_packets;
    std::thread m_thread;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::atomic<bool> m_stop;
    std::queue<std::pair<size_t, size_t>> m_tasks;

};


#endif //PACKETPRESENTER_H
