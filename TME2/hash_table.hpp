#pragma once
#include <vector>
#include <forward_list>
#include <functional>

using namespace std;

template <typename K, typename V>
class HashTable {
public:
    struct Entry {
      K key;
      V value;
    };
    vector<forward_list<Entry>> buckets;
    HashTable(size_t n);
    V* get(const K & key);
    bool put (const K & key, const V & value);
    size_t size() const;
    void grow();
};
    