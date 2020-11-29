//
// Created by Vishakh Nair on 11/24/19.
//


#ifndef PROJECT04_HASHMAP_H
#define PROJECT04_HASHMAP_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <functional>
using namespace std;



// Finds the smallest prime greater than 2 * prime.
inline int nextPrime(int prime) {
    static const int primes[] = {11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421};

    for (int i = 0; i < 9; ++i) {
        if (prime == primes[i]) {
            return primes[i+1];
        }
    }
    return -1;
}



// Both key and value are stored in the slot, so that search will work.
template<typename K, typename V>
struct ValueStore {
    K key;
    V value;
    ValueStore(const K& k, const V& v) : key(k), value(v) {
    }
};


// The Hashmap class.
template<typename K, typename V>
class HashMap {

public:
    HashMap();
    ~HashMap();

    // Adds a key and corresponding value.
    // If the key already exists, its value will be replaced.
    void put(const K& key, const V& value);

    // Returns the value for a key, or nullptr if the key is not found.
    V* get(const K& key) const;

    // Prints the internal state of the hash table.
    void print() const;

    // If the key exists, retrieves it value.
    // If it does not exist, inserts with the key and default V.
    V& operator[] (const K& key);

    int getSize() const {
        return size;
    }

    vector<K> getKeys() const;

private:
    int finalHash(size_t keyHash, int trial) const;    // Probed hash function.
    int fixedHash1(size_t keyHash) const;              // First fixed hash function.
    int fixedHash2(size_t keyHash) const;              // Second fixed hash function.

    void rehash();                              // Rehashes with double the capacity.

    int capacity;                               // Size of the internal array.
    int size;                                   // Number of keys.

    // Internal array of size capacity, each element a pointer to a value with its corresponding key, or nullptr.
    ValueStore<K, V>** array;
};

template<typename K, typename V>
vector<K> HashMap<K, V>::getKeys() const {
    vector<K> keys;
    for (auto it = array; it != array + capacity; ++it) {
        ValueStore<K, V>* vs = *it;
        if (vs != nullptr) {
            keys.push_back(vs->key);
        }
    }
    return keys;
}

// Overload [] operator so as to insert and delete from HashMap
template<typename K, typename V>
V& HashMap<K, V>::operator [] (const K& key) {
    V* value = get(key);
    if (value != nullptr) {
        return *value;
    }

    // Create the map entry if the key doesn't not exist.
    V newValue;
    put(key, newValue);
    return *get(key);
}

template<typename K, typename V>
HashMap<K, V>::HashMap() : capacity(11), size(0)
{
    array = new ValueStore<K, V>*[capacity];
    for (int i = 0; i < capacity; ++i) {
        array[i] = nullptr;
    }
}

template<typename K, typename V>
HashMap<K, V>::~HashMap()
{
    for (int i = 0; i < capacity; ++i) {
        if (array[i] != nullptr) {
            delete array[i];
        }
    }
    delete[] array;
}

template<typename K, typename V>
void HashMap<K, V>::put(const K& key, const V& value)
{
    size_t hashOfKey = hash<K>()(key);

    int trial = 0;
    int index = finalHash(hashOfKey, trial);
    while (array[index] != nullptr) {

        // If the key already exists, overwrite that.
        if (array[index]->key == key) {
            array[index]->value = value;
            return;
        }


        // We check whether there is already an entry for this key.
        ++trial;
        index = finalHash(hashOfKey, trial);
    }

    ValueStore<K, V>* vs = new ValueStore<K, V>(key, value);

    array[index] = vs;
    ++size;


    if (2 * size > capacity) {
        rehash();
    }
}


template<typename K, typename V>
void HashMap<K, V>::rehash()
{
    ValueStore<K, V>** old_array = array;
    int old_capacity = capacity;

    capacity = nextPrime(capacity);
    array = new ValueStore<K, V>*[capacity];
    size = 0;

    for (int i = 0; i < capacity; ++i) {
        array[i] = nullptr;
    }

    for (int i = 0; i < old_capacity; ++i) {
        ValueStore<K, V>* vs = old_array[i];
        if (vs != nullptr) {
            put(vs->key, vs->value);
        }
    }
}

template<typename K, typename V>
V* HashMap<K, V>::get(const K& key) const
{
    size_t hashOfKey = hash<K>()(key);
    int trial = 0;
    while (trial < capacity) {
        int index = finalHash(hashOfKey, trial);
        ValueStore<K, V>* vs = array[index];
        if (vs == nullptr) {
            return nullptr;
        }
        if (vs->key == key) {
            return &vs->value;
        }
        ++trial;
    }
    return nullptr;    // Unlikely, if we rehash properly.
}

// First Hash
template<typename K, typename V>
int HashMap<K, V>::fixedHash1(size_t hashOfKey) const
{
    return hashOfKey % capacity;
}

// Second Hash
template<typename K, typename V>
int HashMap<K, V>::fixedHash2(size_t hashOfKey) const
{
    return 1 + (hashOfKey % (capacity - 1));
}

// Combining the Hashes
template<typename K, typename V>
int HashMap<K, V>::finalHash(size_t hashOfKey, int trial) const
{
    return (fixedHash1(hashOfKey) + trial * fixedHash2(hashOfKey)) % capacity;
}

template<typename K, typename V>
void HashMap<K, V>::print() const
{
    cout << "\n\nTHE HASH TABLE:" << endl;
    for (int i = 0; i < capacity; ++i) {
        ValueStore<K, V>* vs = array[i];
        if (vs != nullptr) {
            cout << "    Slot " << i << ": Key = " << vs->key << ", Value = " << (vs->value) << endl;
        }
    }
    cout << endl;
}




#endif //PROJECT04_HASHMAP_H
