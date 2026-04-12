#include <gtest/gtest.h>
#include <util/PktRingBuffer.h>
#include <util/PacketUtil.h>

class PktRingBufferOverwriteTest
    : public ::testing::TestWithParam<size_t> {
protected:
  static constexpr size_t capacity   = 10;
  static constexpr size_t batch_size = 5;

  PktRingBufferOverwriteTest()
      : buffer(capacity, batch_size) {}

  PktRingBuffer buffer;
};

TEST_P(PktRingBufferOverwriteTest, ReturnsCorrectBatchAfterOverwrite) {
  size_t range = GetParam();

  // First write: [0, batch_size)
  for (size_t i = 0; i < range; ++i) {
    packet_data d;
    d.index = i;
    buffer.add(std::move(d));
  }

  size_t next_end{};
  {
    auto view = buffer.get_packet_batch();
    ASSERT_TRUE(view);
    EXPECT_EQ(view->begin.get_curr_index(), 0);
    EXPECT_EQ(view->end.get_curr_index(), view->begin.get_curr_index() + batch_size);
    next_end = view->end.get_curr_index();
  }

  // Second write: [start, start + batch_size)
  for (size_t i = 0; i < range; ++i) {
    packet_data d;
    d.index = i;
    buffer.add(std::move(d));
  }

  auto view = buffer.get_packet_batch();
  ASSERT_TRUE(view);

  // Expected logical range after overwrite
  const size_t expected_begin = next_end;
  const size_t expected_end = expected_begin + batch_size;
  EXPECT_EQ(view->begin.get_curr_index(), expected_begin);
  EXPECT_EQ(view->end.get_curr_index(), expected_end);
}

INSTANTIATE_TEST_SUITE_P(
    OverwriteScenarios,
    PktRingBufferOverwriteTest,
    ::testing::Values(5, 10, 20, 40));