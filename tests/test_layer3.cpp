//
// Created by jeremiah tesfaye on 9/16/25.
//

#include <gtest/gtest.h>
#include <parsing/ParseDispatcher.h>
#include <packet/PcapFile.h>
#include <layerx/layer3/IPv6.h>
#include <layerx/layer3/IPv4.h>
#include <layerx/Registry.h>

class Layer3Test : public ::testing::Test {
protected:
    Layer3Test() :
    file(ipv4_file_test.c_str()),
    file2(ipv6_file_test.c_str())
    {}

    parse_context context{};
    const std::string ipv4_file_test = "/Users/jt/Desktop/pcap_files/ipv4frags.pcap";
    const std::string ipv6_file_test = "/Users/jt/Desktop/pcap_files/mycap.pcap";

    PcapFile file;
    PcapFile file2;

};

TEST_F(Layer3Test, IPv4ParseTest) {

    int key = protocol::ipv4::iana_number;

    ParseDispatcher parser(registry::get_initial_registry());
    auto raw_data = file.read(0);

    context.offset = 30;

    auto ip = parser(key, raw_data, context);

    ASSERT_NE(ip, nullptr);

    Address src = ip->src();
    Address dest = ip->dest();

    EXPECT_EQ(ip->address_to_string(src), "2.1.1.2");
    EXPECT_EQ(ip->address_to_string(dest), "2.1.1.1");
    EXPECT_EQ(ip->length, 20);

}

TEST_F(Layer3Test, IPv6ParseTest) {

    int key = protocol::ipv6::iana_number;

    ParseDispatcher parser(registry::get_initial_registry());

    context.offset = 30;

    auto raw_data = file2.read(2);

    auto ip = parser(key, raw_data, context);
    Address src = ip->src();
    Address dest = ip->dest();

    std::cout << ip->address_to_string(src) << "\n";

    ASSERT_NE(ip, nullptr);
    EXPECT_EQ(ip->address_to_string(src), "2605:a601:ac61:f600:145:4c82:701d:1168");
    EXPECT_EQ(ip->address_to_string(dest), "2606:4700:4400::ac40:94eb");
    EXPECT_EQ(ip->length, 40);

}
