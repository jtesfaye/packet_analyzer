//
// Created by jeremiah tesfaye on 10/25/25.
//

#ifndef LRUCACHE_H
#define LRUCACHE_H

#include <unordered_map>
#include <list>
#include <util/IContainerType.h>

template<typename CacheType>
class LRUCache : public IContainerType<CacheType> {
public:

    explicit LRUCache(size_t max_limit);

    ~LRUCache() override = default;

    void add(size_t index, CacheType item) override;

    std::optional<std::reference_wrapper<const CacheType>> get(size_t key) override;

    size_t size() const override;

    bool exists(size_t key) const override;


private:

    std::optional<std::reference_wrapper<CacheType>> poll(size_t key) override;

    std::list<size_t>::iterator move_to_front(std::list<size_t>::iterator iter);

    void evict();

    struct Data {

        CacheType item;
        std::list<size_t>::iterator position;

    };

    size_t max_limit;

    std::unordered_map<size_t, Data> cache;
    std::list<size_t> recency_list;

};

#include "LRUCache.tpp"

#endif //LRUCACHE_H
