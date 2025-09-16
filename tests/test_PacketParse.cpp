//
// Created by jeremiah tesfaye on 8/31/25.
//

#include <gtest/gtest.h>
#include <parsing/PacketParse.h>
#include <parsing/ParseDispatcher.h>
#include <packet/PcapFile.h>

class PacketParseTest : public ::testing::Test {
protected:

    PacketParseTest() :
    parser(dlt, flags),
    file(file_name.c_str()) {}

    void SetUp() override {

        raw_data = file.read(0);

    }

    int dlt = DLT_EN10MB;
    u_int8_t flags = 0 | parse::DO_LAYER3 | parse::DO_LAYER4;

    //.pcap file holds packets with ethernet at layer2
    std::string file_name = "/Users/jt/Desktop/pcap_files/ipv4frags.pcap";

    parse::PacketParse parser;
    PcapFile file;
    parse_context context{};
    std::vector<std::byte> raw_data;

};


