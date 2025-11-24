//
// Created by jeremiah tesfaye on 11/21/25.
//

#ifndef STREAM_H
#define STREAM_H

#include <layerx/ProtocolKeys.h>
#include <packet/PacketUtil.h>
#include <vector>
#include <chrono>

using milisec = std::chrono::duration<long long, std::ratio<1,1000>>;

struct StreamStats {

    milisec start_time;
    milisec end_time;
    double rtt;
    double throughput;
    size_t bytes_sent;
    size_t bytes_recieved;
    double avg_packet_size;

    void set_time(bool is_end);

};

using namespace protocol;
class Stream {
public:

    virtual void add_index(const packet::packet_ref&) = 0;

    static  std::shared_ptr<Stream> createConnection(ProtocolKeys type);
    virtual ~Stream();

protected:

    constexpr size_t EPHEMERAL_PORT_START {49151};

    Stream(const ProtocolKeys type)
    : type(type)
    {}

    bool is_ephemeral(const Address &port);

    ProtocolKeys type;
    std::vector<size_t> pkt_idx;
    StreamStats stats{};

};

class TCPStream : public Stream {
public:

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

    TCPStream(ProtocolKeys type);
    ~TCPStream() override;

    //take packet, update state, then add to list
    void add_index(const packet::packet_ref& ref) override;

private:

    void find_state(const packet_ref& ref);
    void set_initial_state(u_int8_t flags, const Side &source, const Side &dest);
    void set_state(u_int8_t flags, const Side &source, const Side &dest);
    void set_client(Side);
    void set_server(Side);

    Side client;
    Side server;

};

#endif //STREAM_H
