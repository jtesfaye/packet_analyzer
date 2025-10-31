//
// Created by jeremiah tesfaye on 9/16/25.
//

#include <gtest/gtest.h>
#include <parsing/ParseDispatcher.h>
#include <packet/PcapFile.h>
#include <layerx/iana_numbers.h>
#include <layerx/layer4/Layer4.h>
#include <layerx/Layer.h>

class Layer4Test : public ::testing::Test {
protected:

    Layer4Test() :
    file(file_name.c_str())
    {}

    parse_context context{};
    const std::string file_name = "/Users/jt/Desktop/pcap_files/mycap.pcap";
    PcapFile file;

};

TEST_F(Layer4Test, DispatchTest) {

    Layer::register_parse_functions();
    const auto& funcs = Layer::get_first_parse_functions();

    // check it’s not empty
    ASSERT_FALSE(funcs.empty());

    // look for TCP entry
    bool found_tcp = false;
    for (const auto& [key, func] : funcs) {
        if (key == layer::iana::TCP) {
            found_tcp = true;
            EXPECT_TRUE(static_cast<bool>(func)); // non-null callable
        }
    }

    EXPECT_TRUE(found_tcp) << "TCP parse function not registered!";

}

TEST_F(Layer4Test, TCPParseSuccess) {

    int key = layer::iana::TCP;

    Layer::register_parse_functions();
    ParseDispatcher<std::unique_ptr<ProtocolDataUnit>> transport_parse(Layer::get_first_parse_functions());

    auto raw_data = file.read(2);

    //pcap header + ethernet header + ipv6 header
    context.offset = sizeof(pcaprec_hdr_t) + 14 + 40;

    auto tcp = transport_parse(key, raw_data, context);

    ASSERT_NE(tcp, nullptr);

    EXPECT_EQ(tcp->src, "57782");
    EXPECT_EQ(tcp->dest, "443");
    EXPECT_EQ(tcp->length, 32);

}

TEST_F(Layer4Test, TCPDetailParse) {

    int key = layer::iana::TCP;

    Layer::register_parse_functions();
    ParseDispatcher<ProtocolDetails> parser(Layer::get_detail_parse_functions());

    auto raw_data = file.read(2);
    context.offset = sizeof(pcaprec_hdr_t) + 14 + 40;

    auto asd = parser(key, raw_data, context);

    std::cout << asd.name << "\n";
    for (const auto& i : asd.fields) {
        std::cout << i << "\n";
    }

}