
#ifndef STREAMIDENTIFIER_H
#define STREAMIDENTIFIER_H

#include <atomic>
#include <functional>
#include <layerx/ProtocolDataUnit.h>
#include <util/PacketUtil.h>
#include <boost/container_hash/hash.hpp>
#include <boost/unordered/concurrent_node_map.hpp>
#include <boost/unordered/concurrent_flat_map.hpp>
#include <boost/unordered/concurrent_flat_set.hpp>
#include <unordered_set>
#include <layerx/ProtocolKeys.h>
#include <stream/Stream.h>

using namespace packet;

struct tuple_5 {
    Address src_port;
    Address src_address;
    Address dest_port;
    Address dest_address;
    std::string_view protocol;
};

namespace std {
    template<>
    struct hash<tuple_5> {
        std::size_t operator()(const tuple_5& t) const noexcept {
            const Address& port_a = t.src_port < t.dest_port ? t.src_port : t.dest_port;
            const Address& port_b = port_a.bytes == t.src_port.bytes ? t.dest_port : t.src_port;
            const Address& ip_c = t.src_address < t.dest_address ? t.src_address : t.dest_address;
            const Address& ip_d = t.src_address.bytes == ip_c.bytes ? t.dest_address : t.src_address;
            size_t seed = 0;
            boost::hash_combine(seed, port_a.bytes);
            boost::hash_combine(seed, port_b.bytes);
            boost::hash_combine(seed, ip_c.bytes);
            boost::hash_combine(seed, ip_d.bytes);
            boost::hash_combine(seed, t.protocol);
            return seed;
        }
    };
}

class StreamTable {
    using flat_map_t = boost::concurrent_flat_map<size_t, int>;
    using node_map_t = boost::concurrent_node_map<size_t, std::shared_ptr<Stream>>;

public:
    size_t add(const packet_ref& ref);
    size_t number_of_connections();
    std::unique_ptr<StreamStats> get_stats(size_t key);

private:
    tuple_5 create_tuple(const packet_ref& ref);
    size_t get_stream(size_t key, std::shared_ptr<Stream>& store) const;
    size_t get_index(size_t, int& store) const;
    const std::unordered_set<ProtocolKeys> can_form_tuple = {ProtocolKeys::TCP, ProtocolKeys::UDP};
    boost::concurrent_node_map<size_t, std::shared_ptr<Stream>> streams;
    boost::concurrent_flat_map<size_t, int> stream_indicies;
    std::hash<tuple_5> hasher {};
    std::atomic<size_t> counter = 0;
    std::mutex lock;
};

#endif //STREAMIDENTIFIER_H