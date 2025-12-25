//
// Created by jeremiah tesfaye on 11/27/25.
//


#include <stream/StreamTable.h>
#include <gtest/gtest.h>
#include <packet/PcapFile.h>
#include <parsing/InitialParser.h>
#include <layerx/ProtocolKeys.h>
#include <print>
class StreamTableTest: public ::testing::Test {
protected:
    StreamTableTest() :
    file(file_name.c_str()),
    parser(static_cast<int>(ProtocolKeys::ETH), 0xff)
    {
        size_t packets = file.get_packet_count();
        for (size_t i = 0; i < packets; i++) {
            auto data = file.read(i);
            refs.push_back(parser.start_extract(std::span(data), i));
        }
    }
    InitialParser parser;
    parse_context context{};
    const std::string file_name = "/Users/jt/Desktop/pcap_files/mycap.pcap";
    PcapFile file;
    std::vector<packet_ref> refs;
};

TEST_F(StreamTableTest, addTest) {
    StreamTable table;
    std::unordered_set<size_t> keys;
    for (auto& ref : refs) {
        auto k = table.add(ref);
        if (ref.layer4 && ref.layer4->key == ProtocolKeys::TCP)
            keys.insert(k);
        ref.stream_index = k;
    }
    ASSERT_EQ(keys.size(), table.number_of_connections());
}

TEST_F(StreamTableTest, getStatsTest) {
    StreamTable table;
    std::unordered_set<size_t> keys;
    for (auto& ref : refs) {
        auto k = table.add(ref);
        if (ref.layer4 && ref.layer4->key == ProtocolKeys::TCP)
            keys.insert(k);
        ref.stream_index = k;
    }
    for (const auto& k : keys) {
        std::unique_ptr<StreamStats> stats = table.get_stats(k);
        if (stats) {
            auto* tcp_stat = dynamic_cast<TCPStream::TCPStats*>(stats.get());
            std::printf("throughput: %f \n", stats->throughput);
            std::printf("rtt: %f\n", tcp_stat->rtt_smoothed);
            std::printf("retranmissions: %lu\n", tcp_stat->retransmissions);
            std::printf("packets in stream: %lu\n", tcp_stat->num_of_packets);
        }
    }
}
