//
// Created by jeremiah tesfaye on 12/25/25.
//

#include <stream/StreamStatistics.h>
#include <format>
#include <iostream>

StreamStatistics::StreamStatistics() {
  data.fill(std::monostate{});
}

stat_t& StreamStatistics::operator[](const StatFields& field) {
  return data[static_cast<size_t>(field)];
}
const stat_t& StreamStatistics::operator[](const StatFields& field) const {
  return data[static_cast<size_t>(field)];
}

const StatContainer &StreamStatistics::get_data() const {
  return data;
}

std::vector<std::string> StreamStatistics::to_string_format() const {
  std::vector<std::string> vec;
  vec.reserve(data.size());
  for (size_t i{}; i < data.size(); i++) {
    if (!std::holds_alternative<std::monostate>(data[i])) {
      auto value = std::visit([](const auto& v) -> std::string {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
          return "";
        } else if constexpr (std::is_same_v<T, EpochTime>) {
          return std::format("{}", v.count());
        } else {
          return std::format("{}", v);
        }
      },data[i]);
      std::string field = std::format("{}:{}",
        field_to_string(static_cast<StatFields>(i)),
        value
      );
      vec.emplace_back(field);
    }
  }
  return vec;
}

std::string_view StreamStatistics::field_to_string(const StatFields field) {
  switch (field) {
    case StatFields::STATE:
      static std::string state = "State";
      return state;

    case StatFields::START_TIME:
      static std::string start_time = "Start time";
      return start_time;

    case StatFields::END_TIME:
      static std::string end_time = "End time";
      return end_time;

    case StatFields::THROUGHPUT:
      static std::string throughput = "Throughput";
      return throughput;

    case StatFields::DATA_BYTES_SENT:
      static std::string dbs = "Data bytes sent";
      return dbs;

    case StatFields::DATA_BYTES_RECEVIED:
      static std::string dbr = "Data bytes received";
      return dbr;

    case StatFields::WIRE_BYTES_SENT:
      static std::string wbs = "Wire bytes sent";
      return wbs;

    case StatFields::WIRE_BYTES_RECEIVED:
      static std::string wbr = "Wire bytes received";
      return wbr;

    case StatFields::AVG_PACKET_SIZE:
      static std::string aps = "Average packet size";
      return aps;

    case StatFields::NUM_OF_PACKETS:
      static std::string nop = "Packets in stream";
      return nop;

    case StatFields::RTT_LAST:
      static std::string rtt_last = "RTT Last";
      return rtt_last;

    case StatFields::RTT_SMOOTHED:
      static std::string rtt_smoothed = "RTT Smoothed";
      return rtt_smoothed;

    case StatFields::RETRANSMISSIONS:
      static std::string retrans = "Retransmissions";
      return retrans;

    default:
      return "";
  }
}

