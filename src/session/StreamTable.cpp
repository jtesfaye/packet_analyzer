
#include <session/StreamTable.h>
#include <mutex>

size_t StreamTable::add(const packet_ref &ref) {

    auto type = ref.layer4->key;
    if (!can_form_tuple.contains(type)) {
        return 0;
    }

    auto tuple = create_tuple(ref);
    std::vector<size_t> vec;
    size_t key = hasher(tuple);

    std::lock_guard guard(lock);
    if (streams.contains(key)) {

        streams.visit(key, [&](node_map_t::value_type& t) {
            t.second->add_index(ref);
        });

    } else {

        std::shared_ptr<Stream> conn = Stream::createConnection(type);
        streams.emplace(key, conn);
        ++counter;
        stream_indicies.emplace(key, counter);
        conn->add_index(ref);
    }

    return key;
}

tuple_5 StreamTable::create_tuple(const packet_ref &ref) {

        return {
            ref.index,
            ref.layer4->src(),
            ref.layer3->src(),
            ref.layer4->dest(),
            ref.layer3->dest(),
            ref.layer4->name()
        };
}


