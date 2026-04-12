//
// Created by jeremiah tesfaye on 10/25/25.
//

#ifndef LRUCACHE_H
#define LRUCACHE_H

#include <unordered_map>
#include <list>
#include <shared_mutex>

template<typename Type>
class LRUCache {
public:
    explicit LRUCache(size_t max_limit);

    void add(size_t index, Type item);
    Type at(size_t key);
    const Type& get(size_t key);
    bool exists(size_t key) const;
    size_t size() const;
    std::shared_ptr<const Type> safe_get(size_t key);

private:
    struct Data {
        Type item;
        std::list<size_t>::iterator position;
    };

    size_t max_limit;
    std::unordered_map<size_t, Data> cache;
    std::list<size_t> recency_list;

    mutable std::shared_mutex rw_lock;
    std::list<size_t>::iterator move_to_front(std::list<size_t>::iterator iter);
    void evict();
};
#include "LRUCache.tpp"

#endif //LRUCACHE_H
