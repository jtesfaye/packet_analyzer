//
// Created by jeremiah tesfaye on 9/16/25.
//

#include <gtest/gtest.h>
#include <parsing/ParseDispatcher.h>
#include <packet/PcapFile.h>
#include <layerx/layer3/Layer3.h>

class Layer3Test : public ::testing::Test {
protected:
    Layer3Test() :
    file(file_name.c_str())
    {}

    parse_context context{};
    const std::string file_name = "/Users/jt/Desktop/pcap_files/ipv4frags.pcap";
    PcapFile file;

};


TEST_F(Layer3Test, IPv4ParseTest) {

    int key = iana::IPV4;

    ParseDispatcher<std::unique_ptr<NetworkPDU>, false> net_parse(
        Layer3::get_all_functions());

    auto raw_data = file.read(0);

    context.offset = 30;

    auto ip = net_parse(key, raw_data, context);

    ASSERT_NE(ip, nullptr);

    EXPECT_EQ(ip->src, "2.1.1.2");
    EXPECT_EQ(ip->dest, "2.1.1.1");
    EXPECT_EQ(ip->length, 20);

}
