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
#include <util/PacketBuffer.h>
#include <packet/PcapFile.h>

class ThreadPoolTest : public ::testing::Test {
protected:

    ThreadPoolTest()
        : init_parser(std::make_shared<InitialParser>(DLT_EN10MB, 0xff))
        , detail_parser(std::make_shared<DetailParser>())
        , buffer(std::make_shared<PacketBuffer<packet_ref>>(10))
        , detail_buffer(std::make_shared<LRUCache<std::vector<ProtocolDetails>>>(10))
        {}

    std::shared_ptr<InitialParser> init_parser;
    std::shared_ptr<DetailParser> detail_parser;
    std::shared_ptr<IContainerType<packet_ref>> buffer;
    std::shared_ptr<IContainerType<std::vector<ProtocolDetails>>> detail_buffer;
    raw_pkt_queue queue;

};

TEST_F(ThreadPoolTest, ProcessesPacketsCorrectly) {

    std::string file_name = "/Users/jt/Desktop/pcap_files/mycap.pcap";
    PcapFile file(file_name);

    PoolInit init {init_parser, detail_parser, buffer, detail_buffer , queue ,5};
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



