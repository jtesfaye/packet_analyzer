//
// Created by jeremiah tesfaye on 12/25/25.
//
#ifndef STREAMSTATISTICS_H
#define STREAMSTATISTICS_H
#include <chrono>
#include <variant>
#include <vector>

enum class StatFields : size_t {
  STATE, START_TIME, END_TIME, THROUGHPUT, DATA_BYTES_SENT, DATA_BYTES_RECEVIED, WIRE_BYTES_SENT,
  WIRE_BYTES_RECEIVED, AVG_PACKET_SIZE, NUM_OF_PACKETS, RTT_LAST, RTT_SMOOTHED, RETRANSMISSIONS, COUNT
};

static constexpr size_t StatFieldCount = static_cast<size_t> (StatFields::COUNT);
using EpochTime = std::chrono::duration<double, std::micro>;
using stat_t = std::variant<std::monostate, size_t, double, EpochTime>;
using StatContainer = std::array<stat_t, StatFieldCount>;

class StreamStatistics {
public:

  StreamStatistics();
  stat_t& operator[](const StatFields& field);
  const stat_t& operator[](const StatFields& field) const;
  std::vector<std::string> to_string_format() const;
  std::vector<std::pair<StatFields, int>> field_pair();
  const StatContainer& get_data() const;

private:
  static std::string_view field_to_string(StatFields field);
  StatContainer data;
};

#endif //STREAMSTATISTICS_H
