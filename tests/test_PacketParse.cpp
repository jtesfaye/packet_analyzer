//
// Created by jeremiah tesfaye on 8/31/25.
//

#include <gtest/gtest.h>
#include <parsing/PacketParse.h>
#include <parsing/ParseDispatcher.h>
#include <packet/PcapFile.h>
#include <print>

class PacketParseTest : public ::testing::Test {
protected:

    PacketParseTest() :
    parser(dlt, flags),
    file(file_name.c_str()) {}

    void SetUp() override {
        for (int i = 0; i < 3; i++) {

            data.push_back(file.read(i));

        }
    }

    void print_arr(const std::array<std::string, 6> &row) {

        std::print("{}, {}, {}, {}, {}, {} \n",
            row[0],
            row[1],
            row[2],
            row[3],
            row[4],
            row[5]);

    }

    int dlt = DLT_EN10MB;
    u_int8_t flags = 0 | parse::DO_LAYER3 | parse::DO_LAYER4;

    //.pcap file holds packets with ethernet at layer2
    std::string file_name = "/Users/jt/Desktop/pcap_files/ipv4frags.pcap";

    parse::PacketParse parser;
    PcapFile file;
    parse_context context{};
    std::vector<std::vector<std::byte>> data;

};

TEST_F(PacketParseTest, ParserTest) {

    int counter = 0;
    for (const auto& i : data) {

        std::pair pair {parser.start_extract(i, counter)};
        print_arr(pair.first.to_array());
        counter++;
    }

    SUCCEED();

}


