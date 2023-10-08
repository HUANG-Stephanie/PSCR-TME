#include "hash_table.hpp"
#include <iostream>

template <typename K, typename V>
HashTable<K,V>::HashTable(size_t n) : buckets(n),sz(n) {}


template <typename K, typename V>
V* HashTable<K, V>::get(const K & key) {
    size_t i = hash<K>{}(key) % buckets.size();
    for (auto & e : buckets[i]) {
        if (e.key == key) {
            return &e.value;
        }
    }
    return nullptr;
}

template <typename K, typename V>
bool HashTable<K, V>::put(const K & key, const V & value) {
    if(sz >= buckets.size() * 8 / 10) {
        grow();
    }
    size_t i = hash<K>{}(key) % buckets.size();
    for (auto & e : buckets[i]) {
        if (e.key == key) {
            e.value = value;
            return true;
        }
    }
    buckets[i].push_front({key, value});
    return false;
}

template <typename K, typename V>
size_t HashTable<K, V>::size() const {
    return buckets.size();
}

template <typename K, typename V>
void HashTable<K, V>::grow() {
    size_t n = buckets.size();
    vector<forward_list<Entry>> new_buckets(n*2);

    for (size_t i = 0; i < n; i++) {
        for (auto & e : buckets[i]) {
            size_t j = hash<K>{}(e.key) % n;
            new_buckets[j].push_front(e);
        }
    }
    buckets = new_buckets;
}

template <typename K, typename V>
typename HashTable<K, V>::iterator HashTable<K, V>::begin() {
    size_t i = 0;
    while (i < sz && buckets[i].empty()) {
        i++;
    }
    if (i < sz) {
        return iterator(buckets, i, buckets[sz].begin());
    }
    return end();
}

template <typename K, typename V>
typename HashTable<K, V>::iterator HashTable<K, V>::end() {
    return iterator(buckets, sz, buckets[sz].end());
}

template class HashTable<string, int>;


