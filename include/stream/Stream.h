//
// Created by jeremiah tesfaye on 11/21/25.
//

#ifndef STREAM_H
#define STREAM_H

#include <atomic>
#include <layerx/layer4/TCP.h>
#include <layerx/ProtocolKeys.h>
#include <util/PacketUtil.h>
#include <vector>
#include <chrono>
#include <deque>
#include <fstream>
#include <variant>
#include <unordered_set>
#include <stream/StreamStatistics.h>

using namespace protocol;
class Stream {
public:
    virtual void add_index(const packet::packet_data&) = 0;
    static  std::shared_ptr<Stream> createConnection(ProtocolKeys type);
    virtual ~Stream() = default;
    virtual StreamStatistics get_stats() = 0;

protected:
    static constexpr size_t EPHEMERAL_PORT_START {49151};
    explicit Stream(const ProtocolKeys type)
    : type(type)
    {}
    bool is_ephemeral(const Address &port);
    ProtocolKeys type;
    std::vector<size_t> pkt_idx;
};

#endif //STREAM_H
