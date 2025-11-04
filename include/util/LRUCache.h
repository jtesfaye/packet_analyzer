//
// Created by jeremiah tesfaye on 10/25/25.
//

#ifndef LRUCACHE_H
#define LRUCACHE_H

#include <unordered_map>
#include <list>
#include <util/IContainerType.h>
#include <semaphore>
#include <shared_mutex>

template<typename CacheType>
class LRUCache : public IContainerType<CacheType> {
public:
    explicit LRUCache(size_t max_limit);

    void add(size_t index, CacheType item) override;
    CacheType get_copy(size_t key) override;
    const CacheType& get(size_t key) override;
    bool exists(size_t key) const override;
    size_t size() const override;
    std::optional<std::reference_wrapper<CacheType>> poll(size_t key) override;

    std::shared_ptr<const CacheType> safe_get(size_t key);

private:
    struct Data {
        CacheType item;
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
