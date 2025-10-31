//
// Created by jeremiah tesfaye on 10/30/25.
//

#pragma once

template<typename CacheType>
LRUCache<CacheType>::LRUCache(size_t max_limit)
: max_limit(max_limit) {

    if (max_limit == 0) {
        throw std::runtime_error("LRUCache: Can't have zero for max_limit");
    }
}

template<typename CacheType>
void LRUCache<CacheType>::add(size_t index, CacheType item) {

    if (recency_list.size() == max_limit) {
        evict();
    }

    recency_list.push_back(index);

    cache[index] = Data{item, std::prev(recency_list.end())};

}

template<typename CacheType>
std::optional<std::reference_wrapper<const CacheType> > LRUCache<CacheType>::get(size_t key) {

    auto it = cache.find(key);

    if (it == cache.end()) {
        return std::nullopt;
    }

    it->second.position = move_to_front(cache[key].position);

    return std::ref(cache[key].item);
}

template<typename CacheType>
size_t LRUCache<CacheType>::size() const {

    return recency_list.size();
}

template<typename CacheType>
bool LRUCache<CacheType>::exists(size_t key) const  {
    return cache.contains(key);
}

template<typename CacheType>
std::optional<std::reference_wrapper<CacheType> > LRUCache<CacheType>::poll(size_t key) {
    return std::nullopt;
}

template<typename CacheType>
std::list<size_t>::iterator LRUCache<CacheType>::move_to_front(const std::list<size_t>::iterator iter) {

    size_t key = *iter;
    recency_list.erase(iter);
    recency_list.push_back(key);
    return std::prev(recency_list.end());

}

template<typename CacheType>
void LRUCache<CacheType>::evict(){

    size_t key_to_remove = recency_list.front();
    cache.erase(key_to_remove);
    recency_list.pop_front();

}


