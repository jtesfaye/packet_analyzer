//
// Created by jeremiah tesfaye on 10/30/25.
//

#pragma once

#include <shared_mutex>

template<typename CacheType>
LRUCache<CacheType>::LRUCache(size_t max_limit)
: max_limit(max_limit)
{
    if (max_limit == 0) {
        throw std::runtime_error("LRUCache: Can't have zero for max_limit");
    }
}

template<typename CacheType>
void LRUCache<CacheType>::add(size_t index, CacheType item) {

    std::unique_lock<std::shared_mutex> lock(rw_lock);

    if (recency_list.size() == max_limit) {
        evict();
    }

    recency_list.push_back(index);
    cache[index] = Data{item, std::prev(recency_list.end())};
}

template<typename CacheType>
CacheType LRUCache<CacheType>::get_copy(size_t key) {
    std::shared_lock<std::shared_mutex> lock(rw_lock);

    auto it = cache.find(key);
    if (it == cache.end()) {
        throw std::runtime_error("LRUCache get_copy: Key does not exist");
    }

    it->second.position = move_to_front(it->second.position);
    return it->second.item;
}

template<typename CacheType>
const CacheType& LRUCache<CacheType>::get(size_t key) {
    std::shared_lock<std::shared_mutex> lock(rw_lock); // shared lock

    auto it = cache.find(key);
    if (it == cache.end()) {
        throw std::runtime_error("LRUCache: Invalid key access");
    }

    it->second.position = move_to_front(it->second.position);
    return it->second.item;
}

template<typename CacheType>
std::shared_ptr<const CacheType> LRUCache<CacheType>::safe_get(size_t key) {

    std::shared_lock<std::shared_mutex> lock(rw_lock); // shared lock

    auto it = cache.find(key);
    if (it == cache.end()) {
        throw std::runtime_error("LRUCache: Invalid key access");
    }

    it->second.position = move_to_front(it->second.position);

    return std::make_shared<const CacheType>(it->second.item);
    
}

template<typename CacheType>
size_t LRUCache<CacheType>::size() const {

    std::shared_lock<std::shared_mutex> lock(rw_lock);

    return recency_list.size();
}

template<typename CacheType>
bool LRUCache<CacheType>::exists(size_t key) const {

    std::shared_lock<std::shared_mutex> lock(rw_lock);

    return cache.contains(key);
}

template<typename CacheType>
std::optional<std::reference_wrapper<CacheType>> LRUCache<CacheType>::poll(size_t key) {

    if (cache.contains(key)) {
        return std::optional{std::ref(cache[key].item)};
    }

    return std::nullopt;
}

template<typename CacheType>
std::list<size_t>::iterator LRUCache<CacheType>::move_to_front(const std::list<size_t>::iterator iter) {

    // NOTE: This must be called with a lock held already
    size_t key = *iter;
    recency_list.erase(iter);
    recency_list.push_back(key);
    return std::prev(recency_list.end());

}

template<typename CacheType>
void LRUCache<CacheType>::evict() {

    size_t key_to_remove = recency_list.front();
    cache.erase(key_to_remove);
    recency_list.pop_front();

}