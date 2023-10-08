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

    class iterator{
    public:
      vector<forward_list<Entry>> & buckets_it;
      size_t index;
      typename forward_list<Entry>::iterator it;

      iterator(vector<forward_list<Entry>> & _buckets, size_t _index, typename forward_list<Entry>::iterator _it) : buckets_it(_buckets), index(_index), it(_it) {}

      iterator & operator++(){
        ++it;
        while(it == buckets_it[index].end() && index != buckets_it.size()){
          ++index;
          it = buckets_it[index].begin();
        }
        return *this;
      }

      bool operator!=(const iterator &other){
        return  it != other.it;
      }

      Entry & operator*(){
        return *it;
      }
    };

    vector<forward_list<Entry>> buckets;
    size_t sz;
    HashTable(size_t n);
    V* get(const K & key);
    bool put (const K & key, const V & value);
    size_t size() const;
    void grow();
    iterator begin();
    iterator end();
};
    