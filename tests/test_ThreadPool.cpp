//
// Created by jeremiah tesfaye on 8/31/25.
//

#include <gtest/gtest.h>
#include <util/ThreadPool.h>
#include <parsing/PacketParse.h>
#include <packet/PcapFile.h>
#include <util/PacketRefBuffer.h>
#include <util/RowFactory.h>
#include <util/PacketObserver.h>
#include <print>

class ThreadPoolTest : public ::testing::Test {
protected:

    ThreadPoolTest()
    : file(file_name.c_str()),
    parser(dlt, flags),
    buffer(file.get_packet_count()),
    observer(buffer),
    observer_thread([&]{observer.wait_for_next();})
    {}

    const int dlt = DLT_EN10MB;
    const u_int8_t flags = 0 | parse::DO_LAYER3 | parse::DO_LAYER4;

    std::string file_name {"/Users/jt/Desktop/pcap_files/mycap.pcap"};
    PcapFile file;
    PacketParse parser;
    ThreadPool pool;
    PacketRefBuffer buffer;
    PacketObserver observer;
    std::thread observer_thread;

    void print_arr(const std::array<const std::string*, 7> row) {

        std::print("{}, {}, {}, {}, {}, {}, {} \n",
            *row[0], *row[1], *row[2], *row[3], *row[4], *row[5], *row[6]);
    }
};


TEST_F(ThreadPoolTest, DoesItInsertIntoBufferCorrectly) {

    for (int i = 0; i < file.get_packet_count(); i++) {

        pool.submit([this, i] {

            const std::vector<std::byte> data = file.read(i);
            auto ref = parser.start_extract(data, i);
            buffer.add(i, std::move(ref));
            observer.notify_if_next(i);

        });
    }

    pool.shutdown();

    observer.set_done();
    observer.notify_all();

    EXPECT_EQ(buffer.size(), 75);

    //visual test
    for (int i = 0; i < buffer.size(); i++) {
        auto ref = RowFactory::create_row(buffer.get_ref(i));
        print_arr(ref.to_array());

    }

    if (observer_thread.joinable()) {
        observer_thread.join();
    }

}