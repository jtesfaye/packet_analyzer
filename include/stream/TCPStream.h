//
// Created by jeremiah tesfaye on 12/25/25.
//

#ifndef TCPSTREAM_H
#define TCPSTREAM_H
#include <stream/Stream.h>

class TCPStream final : public Stream {
public:
  explicit TCPStream(ProtocolKeys type);
  ~TCPStream() override = default;

  void add_index(const packet_data& ref) override;
  StreamStatistics get_stats() override;

private:
  struct tracked_stats {
    EpochTime start_time{0};
    EpochTime end_time{0};
    double throughput{};
    size_t data_bytes_sent{};
    size_t data_bytes_received{};
    size_t wire_bytes_sent{};
    size_t wire_bytes_received{};
    size_t num_of_packets{};
    double avg_packet_size{};
    double rtt_last{};
    double rtt_smoothed{};
    size_t retransmissions{};
  };

  struct Side {
    Address port;
    Address ip;
    tcp::State curr_state{};
  };

  struct UnackedPacket {
    size_t expected_ack;
    EpochTime time;
  };

  void update_state(const packet_data& ref);
  void track_send(u_int32_t seq, size_t payload_len, EpochTime time);
  void track_recv(u_int32_t ack, EpochTime time);
  void track_bytes(u_int16_t src_port, size_t payload_len, size_t wire_len);
  void track_time(EpochTime time);
  void find_state(const TCP *tcpref);
  void set_initial_state(u_int8_t flags, const Side &source, const Side &dest);
  void set_state(u_int8_t flags, const Side &source, const Side &dest);
  void set_client(Side);
  void set_server(Side);
  void update_stats(size_t);
  void update_rtt(double);
  void set_start_time(EpochTime time);
  void set_end_time(EpochTime time);

  u_int32_t init_seq{};
  static constexpr double SRTT_FACTOR {0.125};
  std::deque<UnackedPacket> unacks;
  Side client;
  Side server;
  EpochTime most_recent_timestamp{0};
  tracked_stats stats;
  std::mutex lock;
  std::mutex stat_lock;
};

#endif //TCPSTREAM_H
