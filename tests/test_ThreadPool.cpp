//
// Created by jeremiah tesfaye on 8/31/25.
//

#include <gtest/gtest.h>
#include <parsing/ThreadPool.h>
#include <util/IContainerType.h>
#include <memory>
#include <thread>
#include <chrono>
#include <queue>
#include <util/SparsePacketBuffer.h>
#include <packet/PcapFile.h>
#include <util/PacketObserver.h>

class ThreadPoolTest : public ::testing::Test {
protected:

    ThreadPoolTest()
        : init_parser(std::make_shared<InitialParser>(DLT_EN10MB, 0xff))
        , detail_parser(std::make_shared<DetailParser>())
        , buffer(std::make_shared<SparsePacketBuffer<packet_ref>>(10))
        , detail_buffer(std::make_shared<LRUCache<std::vector<ProtocolDetails>>>(10))
        , observer(std::make_shared<PacketObserver>(*buffer, *detail_buffer))
        , init({init_parser, detail_parser, buffer, detail_buffer , observer ,queue ,5})
        {}

    const std::shared_ptr<InitialParser> init_parser;
    std::shared_ptr<DetailParser> detail_parser;
    std::shared_ptr<SparsePacketBuffer<packet_ref>> buffer;
    std::shared_ptr<LRUCache<std::vector<ProtocolDetails>>> detail_buffer;
    std::shared_ptr<PacketObserver> observer;
    raw_pkt_queue queue;
    PoolInit init;

};

TEST_F(ThreadPoolTest, ProcessesPacketsCorrectly) {

    std::string file_name = "/Users/jt/Desktop/pcap_files/mycap.pcap";
    PcapFile file(file_name);

    ThreadPool pool(init);

    // prepare packets
    for (int i = 0; i < 5; ++i) {
        auto data = file.read(i);

        RawPacket pkt{};
        std::memcpy(pkt.packet,data.data(), data.size());
        pkt.index = i;
        queue.push(pkt);
    }

    pool.shutdown();
}



