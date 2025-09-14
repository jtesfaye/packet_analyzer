//
// Created by jeremiah tesfaye on 7/31/25.
//

#include "MockPcapDumper.h"
#include <gtest/gtest.h>
#include <packet/PcapFile.h>
#include <packet/PcapArray.h>
#include <packet/PacketUtil.h>
#include <fstream>
#include <cstdio>
#include <print>
#include <filesystem>


class PcapFileTest : public ::testing::Test {
protected:

    const char* test_file = "test_output.pcap";
    void TearDown() override {
        std::remove(test_file);
    }

};

TEST_F(PcapFileTest, ConstructAndMapFileSuccessfully) {


    const char* offline_test_file_name = "/Users/jt/Desktop/pcap_files/mycap.pcap";

    PcapFile offlineFile(offline_test_file_name);

    std::vector<std::byte> raw_data = offlineFile.read(0);

    auto* header = reinterpret_cast<const packet::pcaprec_hdr_t*>(raw_data.data());
    EXPECT_EQ(header->incl_len, 60);

    raw_data = offlineFile.read(1);
    auto* header2 = reinterpret_cast<const packet::pcaprec_hdr_t*>(raw_data.data());
    EXPECT_EQ(header2->incl_len, 114);

    raw_data = offlineFile.read(2);
    auto* header3 = reinterpret_cast<const packet::pcaprec_hdr_t*>(raw_data.data());
    EXPECT_EQ(header3->incl_len, 86);

}
