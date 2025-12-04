//
// Created by jeremiah tesfaye on 11/21/25.
//

#ifndef STREAM_H
#define STREAM_H

#include <layerx/ProtocolKeys.h>
#include <packet/PacketUtil.h>
#include <vector>
#include <chrono>
#include <deque>

using EpochTime = std::chrono::duration<double, std::milli>;

struct StreamStats {

    EpochTime start_time;
    EpochTime end_time;
    double throughput;
    size_t bytes_sent;
    size_t bytes_recieved;
    double avg_packet_size;

    void update_base_stat(double throughput, size_t bytes_sent, size_t bytes_received, double avg);
    virtual ~StreamStats() = default;
    void set_time(bool is_end);
    bool end_time_set = false;

};

using namespace protocol;
class Stream {
public:

    virtual void add_index(const packet::packet_ref&) = 0;

    static  std::shared_ptr<Stream> createConnection(ProtocolKeys type);
    virtual ~Stream() = default;
    virtual std::shared_ptr<StreamStats> get_stats() = 0;

protected:

    static constexpr size_t EPHEMERAL_PORT_START {49151};

    explicit Stream(const ProtocolKeys type)
    : type(type)
    {}

    bool is_ephemeral(const Address &port);

    ProtocolKeys type;
    std::vector<size_t> pkt_idx;
    StreamStats stats{};
};

class TCPStream final : public Stream {

    struct TCPStats final : StreamStats {

        u_int32_t init_seq{};
        long double rtt_last{};
        long double rtt_smoothed{};
        size_t retransmissions{};

    };

public:

    explicit TCPStream(ProtocolKeys type);
    ~TCPStream() override = default;

    //take packet, update state, then add to list
    void add_index(const packet::packet_ref& ref) override;
    std::shared_ptr<StreamStats> get_stats() override;

private:

    enum class State {
        UNKNOWN,
        SYN_SENT,
        SYN_RECIEVED,
        ESTABLISHED,
        FIN_WAIT1,
        FIN_WAIT2,
        CLOSE_WAIT,
        CLOSING,
        LAST_ACK,
        TIME_WAIT,
        CLOSED
    };

    struct Side {
        Address port;
        Address ip;
        State curr_state;
    };

    struct UnackedPacket {
        size_t expected_ack;
        EpochTime time;
    };

    void track_send(u_int32_t seq, size_t payload_len, EpochTime time);
    void track_recv(u_int32_t ack, EpochTime time);
    void find_state(const TCP *tcpref);
    void set_initial_state(u_int8_t flags, const Side &source, const Side &dest);
    void set_state(u_int8_t flags, const Side &source, const Side &dest);
    void set_client(Side);
    void set_server(Side);

    static constexpr double SRTT_FACTOR = 0.125;
    std::deque<UnackedPacket> unacks;
    Side client;
    Side server;
    TCPStats tcp_stats{};
    std::mutex lock;

};

#endif //STREAM_H
