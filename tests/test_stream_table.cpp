//
// Created by jeremiah tesfaye on 11/27/25.
//


#include <session/StreamTable.h>
#include <gtest/gtest.h>
#include <packet/PcapFile.h>
#include <parsing/InitialParser.h>
#include <layerx/ProtocolKeys.h>
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

TEST_F(StreamTableTest, CreateTupleTest) {

    StreamTable table;
    std::unordered_set<size_t> keys;
    for (const auto& ref : refs) {
        auto k = table.add(ref);
        keys.insert(k);
    }

    for (auto i : keys) {
        if (int stream{}; table.get_index(i, stream))
            std::cout << stream << "\n";

    }
}
