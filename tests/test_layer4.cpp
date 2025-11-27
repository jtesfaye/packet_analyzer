//
// Created by jeremiah tesfaye on 9/16/25.
//

#include <gtest/gtest.h>
#include <parsing/ParseDispatcher.h>
#include <packet/PcapFile.h>
#include <layerx/Registry.h>
#include <layerx/layer4/TCP.h>

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

    const auto& funcs = registry::get_initial_registry();

    // check it’s not empty
    ASSERT_FALSE(funcs.empty());

    // look for TCP entry
    bool found_tcp = false;
    for (const auto& [key, func] : funcs) {
        if (key == static_cast<int>(protocol::ProtocolKeys::TCP)) {
            found_tcp = true;
            EXPECT_TRUE(static_cast<bool>(func)); // non-null callable
        }
    }

    EXPECT_TRUE(found_tcp) << "TCP parse function not registered!";

}

TEST_F(Layer4Test, TCPParseSuccess) {

    int key = static_cast<int> (ProtocolKeys::TCP);

    ParseDispatcher transport_parse(registry::get_initial_registry());

    auto raw_data = file.read(2);

    //pcap header + ethernet header + ipv6 header
    context.offset = sizeof(pcaprec_hdr_t) + 14 + 40;

    auto tcp = transport_parse(key, raw_data, context);

    ASSERT_NE(tcp, nullptr);

    u_int16_t src;
    Address src_addr = tcp->src();
    std::memcpy(&src, &src_addr.bytes, sizeof(src));

    u_int16_t dest;
    Address dest_addr = tcp->dest();
    std::memcpy(&dest, &dest_addr.bytes, sizeof(dest));

    EXPECT_EQ(src, 57782);
    EXPECT_EQ(dest, 443);
    EXPECT_EQ(tcp->length, 32);

}

TEST_F(Layer4Test, TCPDetailParse) {

    int key = protocol::tcp::iana_number;

    ParseDispatcher parser(registry::get_detail_parse_functions());

    auto raw_data = file.read(2);
    context.offset = sizeof(pcaprec_hdr_t) + 14 + 40;

    auto asd = parser(key, raw_data, context);

    std::cout << asd.name << "\n";
    for (const auto& i : asd.fields) {
        std::cout << i << "\n";
    }

}