//
// Created by jeremiah tesfaye on 9/16/25.
//

#include <gtest/gtest.h>
#include <parsing/ParseDispatcher.h>
#include <packet/PcapFile.h>
#include <layerx/layer4/Layer4.h>

class Layer4Test : public ::testing::Test {
protected:
    Layer4Test() :
    file(file_name.c_str())
    {}

    parse_context context{};
    const std::string file_name = "/Users/jt/Desktop/pcap_files/mycap.pcap";
    PcapFile file;

};

TEST_F(Layer4Test, TCPParseSuccess) {

    int key = layer::iana::TCP;

    ParseDispatcher<std::unique_ptr<TransportPDU>, false> transport_parse(
        Layer4::get_all_functions());

    auto raw_data = file.read(2);

    //pcap header + ethernet header + ipv6 header
    context.offset = sizeof(pcaprec_hdr_t) + 14 + 40;

    auto tcp = transport_parse(key, raw_data, context);

    ASSERT_NE(tcp, nullptr);

    EXPECT_EQ(tcp->src, "57782");
    EXPECT_EQ(tcp->dest, "443");
    EXPECT_EQ(tcp->length, 32);

}