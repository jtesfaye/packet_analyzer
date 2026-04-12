//
// Created by jeremiah tesfaye on 8/31/25.
//

#include <gtest/gtest.h>
#include <session/CaptureEngine.h>
#include <memory>
#include <thread>
#include <chrono>
#include <queue>
#include <util/PktRingBuffer.h>
#include <packet/PcapFile.h>
#include <chrono>

class CaptureEngineTest : public ::testing::Test {
protected:

    CaptureEngineTest()
    : file(std::make_shared<PcapFile>("/Users/jt/Desktop/pcap_files/mycap.pcap"))
    {
        init.layer2_type = DLT_EN10MB;
        init.flags = 0xff;
        init.cache_size_limit = 300;
        init.thread_count = std::thread::hardware_concurrency();
        init.raw_pkt_writer = PacketCapture::createOfflineCapture(ci);
    }

    std::vector<size_t> pkt_ids;
    const std::shared_ptr<PcapFile> file;
    raw_pkt_queue queue;
    std::unique_ptr<CaptureEngine> engine;
    SessionCallback func = [this] (const PktRingBuffer::PktView& view) {
        for (auto iter = view.begin; iter != view.end; ++iter) {
            pkt_ids.push_back(iter->index);
        }
    };
    CaptureInit ci {nullptr, file, queue};
    EngineInit init;
};

TEST_F(CaptureEngineTest, OrderPacketsAndCallback) {
    std::thread capture_thread {[this] () {
        engine = std::make_unique<CaptureEngine>(init, func);
        engine->start();
    }};
    capture_thread.detach();
    std::this_thread::sleep_for(std::chrono::seconds(15));
    engine->stop();
    engine->shutdown();
    if (capture_thread.joinable()) {
        capture_thread.join();
    }
    for (size_t i{}; i < pkt_ids.size(); i++) {
        ASSERT_EQ(pkt_ids[i], i);
    }
}



