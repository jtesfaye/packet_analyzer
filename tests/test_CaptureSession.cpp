//
// Created by jeremiah tesfaye on 10/29/25.
//

#include <gtest/gtest.h>
#include <session/CaptureSession.h>
#include <chrono>

class CaptureSessionTest : public ::testing::Test {
protected:

    int packet_count = 10;
    int capture_size = 255;

    const CaptureConfig config {
        CaptureMode::Online,
        "en0",
        packet_count,
        capture_size,
        0xff,
        0xff,
        ""
    };

};

TEST_F(CaptureSessionTest, ConstructAndDeconstruct) {

    CaptureSession session(config);

}

TEST_F(CaptureSessionTest, StartCaptureSuccessfully) {

    CaptureSession session(config);

    std::thread session_thread{[&] () {
        session.start_session();
    }};

    session.send_command(SessionCommand::start());
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    session.send_command(SessionCommand::stop());
    session.send_command(SessionCommand::end());

    ASSERT_EQ(session.get_buffer().size(), 10);
    ASSERT_EQ(session.get_cache().size(), 10);

    session_thread.join();

}

