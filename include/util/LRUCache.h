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

    explicit LRUCache(size_t max_limit)
    : max_limit(max_limit) {

        if (max_limit == 0) {
            throw std::runtime_error("LRUCache: Can't have zero for max_limit");
        }
    }

    ~LRUCache() override = default;

    void add(size_t index, CacheType item) override  {

        if (recency_list.size() == max_limit) {
            evict();
        }

        recency_list.push_back(index);

        cache[index] = Data{item, std::prev(recency_list.end())};

    }

    std::optional<CacheType> get(size_t key) override {

        if (!cache.contains(key)) {
            return std::nullopt;
        }

        cache[key].position = move_to_front(cache[key].position);

        return cache[key].item;
    }

    size_t size() const override {
        return recency_list.size();
    }

    bool exists(size_t key) const override {
        return cache.contains(key);
    }


private:

    std::optional<CacheType> poll(size_t key) override {
        return std::nullopt;
    }

    std::list<size_t>::iterator move_to_front(const std::list<size_t>::iterator iter) {

        size_t key = *iter;

        recency_list.erase(iter);
        recency_list.push_back(key);
        return std::prev(recency_list.end());

    }

    void evict() {

        size_t key_to_remove = recency_list.front();
        cache.erase(key_to_remove);
        recency_list.pop_front();

    }

    struct Data {

        CacheType item;
        std::list<size_t>::iterator position;

    };

    size_t max_limit;

    std::unordered_map<size_t, Data> cache;
    std::list<size_t> recency_list;

};

#endif //LRUCACHE_H
