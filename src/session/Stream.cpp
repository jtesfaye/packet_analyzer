//
// Created by jeremiah tesfaye on 11/21/25.
//

#include <layerx/layer4/TCP.h>
#include <session/Stream.h>
#include <chrono>
#include <iostream>

std::shared_ptr<Stream> Stream::createConnection(const ProtocolKeys type) {

    switch (type) {
        case ProtocolKeys::TCP:
            return std::make_unique<TCPStream>(type);
        default:
            return nullptr;
    }
}

bool Stream::is_ephemeral(const Address &port) {

    u_int16_t portnum;
    std::memcpy(&portnum, port.bytes.data(), sizeof(portnum));
    return portnum >= EPHEMERAL_PORT_START;
}

TCPStream::TCPStream(const ProtocolKeys type)
: Stream(type) {
    pkt_idx.reserve(100);
    stats.set_time(false);

}

void TCPStream::add_index(const packet_ref &ref) {

    using namespace std::chrono;

    if (ref.layer4->key != ProtocolKeys::TCP) {
        throw std::runtime_error("Wrong protocol");
    }
    const auto hdr = dynamic_cast<TCP*>(ref.layer4.get());

    std::lock_guard guard(lock);
    find_state(hdr);

    auto epoch = seconds{ref.time.ts_sec} + milliseconds{ref.time.ts_usec};
    track_send(hdr->seq_number,1, epoch);
    track_recv(hdr->ack_number, epoch);

    pkt_idx.push_back(ref.index);
}

void TCPStream::find_state(const TCP* tcpref) {
    using namespace protocol::tcp::flags;

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
    bool from_client = (source.port.bytes == client.port.bytes);
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
                stats.set_time(true);
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
    client.port = val.port;
    client.ip = val.ip;
}

void TCPStream::set_server(Side val) {
    server.port = val.port;
    server.ip = val.ip;
}

void StreamStats::set_time(bool is_end) {

    using namespace std::chrono;
    const auto now = system_clock::now().time_since_epoch();
    const auto miliseconds_since_epoch = duration<double, std::milli>(now);

    if (is_end) {
        end_time = miliseconds_since_epoch;
        end_time_set = true;
    } else {
        start_time = miliseconds_since_epoch;
    }
}

void StreamStats::update_base_stat(double throughput, size_t bytes_sent, size_t bytes_received, double avg) {
    this->throughput = throughput;
    this->bytes_sent = bytes_sent;
    this->bytes_recieved = bytes_received;
    this->avg_packet_size = avg;
}

std::shared_ptr<StreamStats> TCPStream::get_stats() {

    using namespace std::chrono;
    double now{};

    if (tcp_stats.end_time_set) {
        now = tcp_stats.end_time.count();
    } else {
        now = duration<double, std::milli>(system_clock::now().time_since_epoch()).count() / 1000;
    }

    double throughput = tcp_stats.bytes_sent + tcp_stats.bytes_recieved / now;
    double avg = tcp_stats.bytes_sent + tcp_stats.bytes_recieved / pkt_idx.size();

    tcp_stats.update_base_stat(throughput, tcp_stats.bytes_sent, tcp_stats.bytes_recieved, avg);

    return std::make_shared<TCPStats>(tcp_stats);
}

void TCPStream::track_send(const u_int32_t seq, const size_t payload_len, const EpochTime time) {

    if (!tcp_stats.init_seq) {
        tcp_stats.init_seq = seq;
    }

    if (!unacks.empty()) {
        if (seq < unacks.back().expected_ack) {
            tcp_stats.retransmissions += 1;
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
            tcp_stats.rtt_last = rtt;

            if (tcp_stats.rtt_smoothed == 0.0) {
                tcp_stats.rtt_smoothed = rtt;
            } else {
                tcp_stats.rtt_smoothed = (1 - SRTT_FACTOR) * tcp_stats.rtt_smoothed + SRTT_FACTOR * rtt;
            }

            unacks.pop_front();
        } else {
            break;
        }
    }
}





