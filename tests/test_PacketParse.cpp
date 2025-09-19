//
// Created by jeremiah tesfaye on 8/31/25.
//

#include <gtest/gtest.h>
#include <parsing/PacketParse.h>
#include <packet/PcapFile.h>
#include <util/RowFactory.h>
#include <print>


class PacketParseTest : public ::testing::Test {
protected:

    PacketParseTest() :
    parser(dlt, flags),
    file(file_name.c_str()) {}

    void SetUp() override {
        for (int i = 0; i < file.get_packet_count(); i++) {

            data.push_back(file.read(i));

        }
    }

    void print_arr(const std::array<const std::string*, 7> row) {

        std::print("{}, {}, {}, {}, {}, {}, {} \n",
            *row[0], *row[1], *row[2], *row[3], *row[4], *row[5], *row[6]);
    }

    int dlt = DLT_EN10MB;
    u_int8_t flags = 0 | parse::DO_LAYER3 | parse::DO_LAYER4;

    //.pcap file holds packets with ethernet at layer2
    std::string file_name = "/Users/jt/Desktop/pcap_files/mycap.pcap";

    PacketParse parser;
    PcapFile file;
    parse_context context{};
    std::vector<std::vector<std::byte>> data;

};

TEST_F(PacketParseTest, ParserTest) {

    int counter = 0;
    for (const auto& i : data) {

        packet_ref ref {parser.start_extract(i, counter)};
        row_entry row = RowFactory::create_row(ref);
        print_arr(row.to_array());
        counter++;
    }

    SUCCEED();
}


