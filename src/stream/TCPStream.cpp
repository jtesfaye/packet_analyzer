//
// Created by jeremiah tesfaye on 12/25/25.
//
#include <cassert>
#include <stream/TCPStream.h>

TCPStream::TCPStream(const ProtocolKeys type)
: Stream(type) {
  pkt_idx.reserve(100);
  client.curr_state = tcp::State::UNKNOWN;
  server.curr_state = tcp::State::UNKNOWN;
}

void TCPStream::add_index(const packet_data &ref) {
  if (ref.layer4->key != ProtocolKeys::TCP) {
    throw std::runtime_error("Wrong protocol");
  }
  std::lock_guard guard(lock);
  update_state(ref);
  pkt_idx.push_back(ref.index);
}

void TCPStream::update_state(const packet_data &ref) {
  using namespace std::chrono;
  const auto hdr = dynamic_cast<TCP*>(ref.layer4.get());
  most_recent_timestamp = seconds{ref.time.ts_sec} + microseconds{ref.time.ts_usec};
  u_int16_t src_port{};
  std::memcpy(&src_port, hdr->src().bytes.data(), hdr->src().size);
  find_state(hdr);
  track_bytes(src_port, ref.payload_length, ref.wire_length);
  track_send(hdr->seq_number, ref.payload_length, most_recent_timestamp);
  track_recv(hdr->ack_number, most_recent_timestamp);
  track_time(most_recent_timestamp);
}

void TCPStream::track_time(EpochTime time) {
  if (pkt_idx.empty()) {
    set_start_time(time);
  }
  if (client.curr_state == tcp::State::CLOSED
      && server.curr_state == tcp::State::CLOSED
      && stats.end_time.count() == 0) {
    set_end_time(time);
      }
}

void TCPStream::track_bytes(u_int16_t src_port, size_t payload_len, size_t wire_len) {
  u_int16_t client_src{};
  u_int16_t server_src{};
  std::memcpy(&client_src, client.port.bytes.data(), client.port.size);
  std::memcpy(&server_src, server.port.bytes.data(), server.port.size);
  if (src_port == client_src) {
    stats.data_bytes_sent += payload_len;
    stats.wire_bytes_sent += wire_len;
  } else {
    stats.data_bytes_received += payload_len;
    stats.wire_bytes_received += wire_len;
  }
}

void TCPStream::find_state(const TCP* tcpref) {
  using namespace protocol::tcp::flags;
  using namespace tcp;
  Side source {tcpref->src(), tcpref->src(), State::UNKNOWN};
  Side dest {tcpref->dest(), tcpref->dest(), State::UNKNOWN};
  if (pkt_idx.empty()) {
    set_initial_state(tcpref->flags, source, dest);
  } else {
    set_state(tcpref->flags, source, dest);
  }
}

/*
 * This function is to determine the state in a newly discovered connection
 */
void TCPStream::set_initial_state(const u_int8_t flags, const Side &source, const Side &dest) {
  using namespace protocol::tcp::flags;
  using namespace tcp;
  if (flags & SYN) {
    set_client(source);
    set_server(dest);
    client.curr_state = State::SYN_SENT;
    server.curr_state = State::SYN_RECIEVED;
    return;
  }
  if (flags & (SYN | ACK)) {
    set_server(source);
    set_client(dest);
    client.curr_state = State::ESTABLISHED;
    server.curr_state = State::ESTABLISHED;
    return;
  }
  if (flags & ACK) {
    const bool src_is_eph = is_ephemeral(source.port);
    const bool dest_is_eph = is_ephemeral(dest.port);
    if (src_is_eph && !dest_is_eph) {
      set_client(source);
      set_server(dest);
    } else {
      set_client(dest);
      set_server(source);
    }
    client.curr_state = State::ESTABLISHED;
    server.curr_state = State::ESTABLISHED;
  }
}

/*
 * This function assumes that whoever is the client and whoever is the server has been correctly established.
 */
void TCPStream::set_state(u_int8_t flags, const Side &source, const Side &dest) {
    using namespace protocol::tcp::flags;
    using namespace tcp;

    bool from_client = source.port.bytes == client.port.bytes;
    Side& snd = from_client ? client : server;   // sender
    Side& rcv = from_client ? server : client;   // receiver
    State& S = snd.curr_state;

    if (S == State::SYN_SENT && (flags & SYN) && (flags & ACK)) {
        S = State::ESTABLISHED;
        rcv.curr_state = State::ESTABLISHED;
        return;
    }
    if (S == State::SYN_RECIEVED && (flags & ACK)) {
        S = State::ESTABLISHED;
        rcv.curr_state = State::ESTABLISHED;
        return;
    }
    if (flags & FIN) {
        switch (S) {
            case State::ESTABLISHED:
                S = State::FIN_WAIT1;
            break;
            case State::FIN_WAIT1:
                S = State::CLOSING;
            break;
            case State::CLOSE_WAIT:
                S = State::LAST_ACK;
            break;
            default:
                break;
        }
    }
    if (flags & ACK) {
        switch (S) {
            case State::FIN_WAIT1:
                S = State::FIN_WAIT2;
            break;

            case State::CLOSING:
                S = State::TIME_WAIT;
            break;
            case State::LAST_ACK:
                S = State::CLOSED;
            break;
            default:
                break;
        }
    }
    if (flags & RST) {
        S = State::CLOSED;
        rcv.curr_state = State::CLOSED;
    }
}

void TCPStream::set_client(Side val) {
    u_int16_t port;
    std::memcpy(&port, val.port.bytes.data(), sizeof(u_int16_t));
    client.port = val.port;
    client.ip = val.ip;
}

void TCPStream::set_server(Side val) {
    u_int16_t port;
    std::memcpy(&port, val.port.bytes.data(), sizeof(u_int16_t));
    server.port = val.port;
    server.ip = val.ip;
}

void TCPStream::set_start_time(EpochTime time) {
    assert(stats.start_time.count() == 0);
    stats.start_time = time;
}

void TCPStream::set_end_time(EpochTime time) {
    assert(time.count() > stats.start_time.count());
    assert(stats.end_time.count() == 0);
    stats.end_time = time;
}

void TCPStream::update_stats(size_t pkts_in_stream) {
    using namespace std::chrono;
    const auto total_bytes = static_cast<double>(stats.wire_bytes_sent + stats.wire_bytes_received);
    const auto end_tp = stats.end_time.count() == 0 ? system_clock::now().time_since_epoch() : stats.end_time;
    const auto dur = duration_cast<duration<double>>(end_tp - stats.start_time);
    const double seconds = dur.count();
    stats.throughput = seconds > 0 ? total_bytes / seconds : 0.0;
    stats.avg_packet_size = total_bytes / static_cast<double>(pkts_in_stream);
    stats.num_of_packets = pkts_in_stream;
}

void TCPStream::update_rtt(const double rtt) {
    stats.rtt_last = rtt;
    if (stats.rtt_smoothed == 0.0) {
        stats.rtt_smoothed = rtt;
    } else {
        stats.rtt_smoothed = (1 - SRTT_FACTOR) * stats.rtt_smoothed + SRTT_FACTOR * rtt;
    }
}

StreamStatistics TCPStream::get_stats() {
    std::lock_guard lock(stat_lock);
    update_stats(pkt_idx.size());

    StreamStatistics s;

    s[StatFields::START_TIME] = stats.start_time;
    s[StatFields::END_TIME] = stats.end_time;
    s[StatFields::THROUGHPUT] = stats.throughput;
    s[StatFields::DATA_BYTES_SENT] = stats.data_bytes_sent;
    s[StatFields::DATA_BYTES_RECEVIED] = stats.data_bytes_received;
    s[StatFields::WIRE_BYTES_SENT] = stats.data_bytes_sent;
    s[StatFields::WIRE_BYTES_RECEIVED] = stats.wire_bytes_received;
    s[StatFields::NUM_OF_PACKETS] = stats.num_of_packets;
    s[StatFields::AVG_PACKET_SIZE] = stats.avg_packet_size;
    s[StatFields::RTT_LAST] = stats.rtt_last;
    s[StatFields::RTT_SMOOTHED] = stats.rtt_smoothed;
    s[StatFields::RETRANSMISSIONS] = stats.retransmissions;
    return s;
}

void TCPStream::track_send(const u_int32_t seq, const size_t payload_len, const EpochTime time) {
    if (!init_seq) {
        init_seq = seq;
    }
    if (!unacks.empty()) {
        if (seq < unacks.back().expected_ack) {
            stats.retransmissions += 1;
        }
    }
    if (payload_len != 0) {
        u_int32_t expected_ack = seq + payload_len;
        unacks.push_back({expected_ack, time});
    }
}

void TCPStream::track_recv(const u_int32_t ack, const EpochTime time) {
    while (!unacks.empty()) {
        UnackedPacket &pkt = unacks.front();
        if (ack >= pkt.expected_ack) {
            const double rtt = (time - pkt.time).count();
            update_rtt(rtt);
            unacks.pop_front();
        } else {
            break;
        }
    }
}
