//
// Created by jeremiah tesfaye on 9/16/25.
//

#include <gtest/gtest.h>
#include <parsing/ParseDispatcher.h>
#include <packet/PcapFile.h>
#include <layerx/layer2/Layer2.h>

class Layer2Test : public ::testing::Test {
protected:
    Layer2Test() :
    file(file_name.c_str())
    {}

    parse_context context{};
    const std::string file_name = "/Users/jt/Desktop/pcap_files/ipv4frags.pcap";
    PcapFile file;

};

TEST_F(Layer2Test, EthernetTest) {

    ParseDispatcher<std::unique_ptr<ProtocolDataUnit>, true> link_parser(
        DLT_EN10MB,
        Layer2::get_all_functions());

    auto raw_data = file.read(0);

    context.offset = 16;

    std::unique_ptr<ProtocolDataUnit> ether = link_parser(raw_data, context);

    ASSERT_NE(ether, nullptr);

    EXPECT_EQ(ether->length, 14);
    EXPECT_EQ(ether->src, "08:00:27:fc:6a:c9");
    EXPECT_EQ(ether->dest, "08:00:27:e2:9f:a6");
    EXPECT_EQ(context.next_type, 0x0800);

}