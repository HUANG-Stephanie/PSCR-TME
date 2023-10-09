#pragma once
#include <vector>
#include <forward_list>
#include <functional>
#include <iostream>

using namespace std;

template <typename K, typename V>
class HashTable {
public:
    struct Entry {
      K key;
      V value;
    };

    vector<forward_list<Entry>> buckets;
    size_t sz;
    HashTable(size_t n);
    V* get(const K & key);
    bool put (const K & key, const V & value);
    size_t size() const;
    void grow();
    
    class iterator{
      public:
      vector<forward_list<Entry>> & buckets_it;
      size_t index;
      typename forward_list<Entry>::iterator it;

      iterator(vector<forward_list<Entry>> & _buckets, size_t _index, typename forward_list<Entry>::iterator _it) : buckets_it(_buckets),index(_index),it(_it){}

      iterator & operator++(){
        ++it;
        while(it == buckets_it[index].end() && index < buckets_it.size()){
          ++index;
          it = buckets_it[index].begin();
        }
        return *this;
      }

      bool operator!=(const iterator &other){
        return it != other.it && index != other.index;
      }

      Entry & operator*(){
        return *it;
      }
    };
    
    iterator begin(){
      size_t i;
      for(i = 0; i<sz,buckets[i].empty();i++);
      return iterator(buckets,i,buckets[i].begin());
    };

    iterator end(){
      return iterator(buckets,sz-1,buckets[sz-1].begin());
    };

    size_t count (iterator begin, iterator end){
      size_t elt = 0;
      for(iterator it = begin; it != end; ++it, ++elt);
      return elt;
    }

    template <typename T>
    size_t count_if_equal (iterator begin, iterator end, const T & val){
      for(iterator it = begin; it != end; ++it){
        Entry& e = *it;
        if(e.key == val){
          return e.value;
        }
      }
      return 0;
    }
};
    