//
// Created by jeremiah tesfaye on 9/3/25.
//

#include <gtest/gtest.h>
#include <layer3_protocols/Layer3.h>
#include <packet/PcapFile.h>

class protocol_test : public ::testing::Test {

};


//testing ipv4
//need a singular packet
//so open pcap
TEST_F(protocol_test, ipv4_test) {

    const char* offline_file = "/Users/jt/Desktop/pcap_files/ipv4frags.pcap";

    PcapFile file(offline_file);
    const std::vector<std::byte> raw_data = file.read(0);

    EXPECT_EQ(raw_data.size(), 1010 + 16);

    parse_context cxt {};

    cxt.offset = 16  + 14;
    auto ref = IPv4_functions::IPv4_parse(raw_data, cxt);

    EXPECT_EQ(cxt.length, 20);
    EXPECT_EQ(cxt.next_type, 1);

    std::cout << cxt.entry.src << std::endl;
    std::cout << cxt.entry.dest <<std::endl;


}
