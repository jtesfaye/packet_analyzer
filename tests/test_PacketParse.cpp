//
// Created by jeremiah tesfaye on 8/31/25.
//

#include <gtest/gtest.h>
#include <parsing/PacketParse.h>
#include <parsing/ParseDispatcher.h>

class PacketParseTest : public ::testing::Test {
protected:

    PacketParseTest() :
    parser(dlt, flags) {}

    int dlt = DLT_EN10MB;
    u_int8_t flags = 0 | parse::DO_LAYER3 | parse::DO_LAYER4;
    parse::PacketParse parser;

};


TEST_F(PacketParseTest, ParseSuccessfully) {


    SUCCEED();

}

