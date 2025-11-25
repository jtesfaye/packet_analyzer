
#ifndef STREAMIDENTIFIER_H
#define STREAMIDENTIFIER_H

#include <atomic>
#include <functional>
#include <layerx/ProtocolDataUnit.h>
#include <packet/PacketUtil.h>
#include <boost/container_hash/hash.hpp>
#include <boost/unordered/concurrent_node_map.hpp>
#include <boost/unordered/concurrent_flat_map.hpp>
#include <boost/unordered/concurrent_flat_set.hpp>
#include <unordered_set>
#include <layerx/ProtocolKeys.h>
#include <session/Stream.h>

using namespace packet;

struct tuple_5 {
    size_t ref_index;
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

            const auto& [
                _,
                src_p,
                src_addr,
                dest_port,
                dest_addr,
                protocol
                ] = t;

            size_t seed = 0;

            boost::hash_combine(seed, src_p.bytes);
            boost::hash_combine(seed, src_addr.bytes);
            boost::hash_combine(seed, dest_port.bytes);
            boost::hash_combine(seed, dest_addr.bytes);
            boost::hash_combine(seed, protocol);

            return seed;
        }
    };
}


class StreamTable {

    using flat_map_t = boost::concurrent_flat_map<size_t, int>;
    using node_map_t = boost::concurrent_node_map<size_t, std::shared_ptr<Stream>>;

public:

    size_t add(const packet_ref& ref);
    int get_index(size_t);

private:

    tuple_5 create_tuple(const packet_ref& ref);
    const std::unordered_set<ProtocolKeys> can_form_tuple = {ProtocolKeys::TCP, ProtocolKeys::UDP};
    boost::concurrent_node_map<size_t, std::shared_ptr<Stream>> streams;
    boost::concurrent_flat_map<size_t, int> stream_indicies;
    std::hash<tuple_5> hasher {};
    std::atomic<size_t> counter = 0;
    std::mutex lock;

};


#endif //STREAMIDENTIFIER_H