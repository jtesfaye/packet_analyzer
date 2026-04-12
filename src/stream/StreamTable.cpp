
#include <iostream>
#include <stream/StreamTable.h>
#include <mutex>

size_t StreamTable::add(const packet_data &ref) {
    if (!ref.layer4) {
        return {};
    }
    auto type = ref.layer4->key;
    if (!can_form_tuple.contains(type)) {
        return {};
    }

    auto tuple = create_tuple(ref);
    size_t key = hasher(tuple);

    std::lock_guard guard(lock);
    if (streams.contains(key)) { //We are adding to an already established connection
        streams.visit(key, [&](node_map_t::value_type& t) {
            t.second->add_index(ref);
        });
    } else { //New connection discovered
        std::shared_ptr<Stream> conn = Stream::createConnection(type);
        if (!conn) {
            return {};
        }
        streams.emplace(key, conn);
        ++counter;
        stream_indicies.emplace(key, counter);
        conn->add_index(ref);
    }
    return key;
}

StreamStatistics StreamTable::get_stats(size_t key) {
    StreamStatistics stats;
    streams.cvisit(key, [&stats] (const node_map_t::value_type& t) {
        stats = t.second->get_stats();
    });
    return stats;
}

size_t StreamTable::get_index(size_t key, int& store) const {
    return stream_indicies.cvisit(key, [&store] (const flat_map_t::value_type& t) {
        store = t.second;
    });
}

size_t StreamTable::number_of_connections() {
    return counter;
}

size_t StreamTable::get_stream(size_t key, std::shared_ptr<Stream>&store) const {
    return streams.cvisit(key, [&store] (node_map_t::value_type const& t) {
        store = t.second;
    });
}

tuple_5 StreamTable::create_tuple(const packet_data &ref) {
        return {
            ref.layer4->src(),
            ref.layer3->src(),
            ref.layer4->dest(),
            ref.layer3->dest(),
            ref.layer4->name()
        };
}






