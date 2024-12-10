#ifndef HASHTABLEDICTIONARY_H
#define HASHTABLEDICTIONARY_H

#include "IDictionary.h"
#include "HashTable.h"
#include <utility>

template <typename TKey, typename TElement>
class HashTableDictionary : public IDictionary<TKey, TElement> {
private:
    HashTable<TKey, TElement> hashTable;

    class HashTableDictionaryIterator : public IDictionary<TKey, TElement>::Iterator {
    private:
        typename HashTable<TKey, TElement>::Iterator innerIterator;

    public:
        HashTableDictionaryIterator(const typename HashTable<TKey, TElement>::Iterator& it) : innerIterator(it) {}

        std::pair<TKey, TElement> operator*() const override { return *innerIterator; }
        HashTableDictionaryIterator& operator++() override { ++innerIterator; return *this; }
        bool operator==(const typename IDictionary<TKey, TElement>::Iterator& other) const override {
            const auto* otherCasted = dynamic_cast<const HashTableDictionaryIterator*>(&other);
            return otherCasted && innerIterator == otherCasted->innerIterator;
        }
        bool operator!=(const typename IDictionary<TKey, TElement>::Iterator& other) const override { return !(*this == other); }
    };

public:
    explicit HashTableDictionary(size_t initialCapacity = 25) : hashTable(initialCapacity) {}

    size_t GetCount() const override { return hashTable.GetCount(); }
    size_t GetCapacity() const override { return hashTable.GetCapacity(); }

    // TElement Get(const TKey& key) const override { return hashTable.Get(key); }

    TElement& Get(const TKey& key) const override { return hashTable.Get(key); }

    void Add(const TKey& key, const TElement& element) override {
        hashTable.Add(key, element);
    }

    void Remove(const TKey& key) override {
        hashTable.Remove(key);
    }

    bool ContainsKey(const TKey& key) const override {
        return hashTable.ContainsKey(key);
    }

    ShrdPtr<Sequence<std::pair<TKey, TElement>>> GetAllItems() const override {
        return hashTable.GetAllItems();
    }

    ShrdPtr<typename IDictionary<TKey, TElement>::Iterator> begin() override {
        return ShrdPtr<typename IDictionary<TKey, TElement>::Iterator>(
            new HashTableDictionaryIterator(hashTable.begin()));
    }

    ShrdPtr<typename IDictionary<TKey, TElement>::Iterator> end() override {
        return ShrdPtr<typename IDictionary<TKey, TElement>::Iterator>(
            new HashTableDictionaryIterator(hashTable.end()));
    }

    TElement& operator[](const TKey& key) {
        if (!ContainsKey(key)) {
            throw std::out_of_range("Key does not exist in HashTableDictionary");
        }
        return hashTable.Get(key);
    }

    const TElement& operator[](const TKey& key) const {
        if (!ContainsKey(key)) {
            throw std::out_of_range("Key does not exist in HashTableDictionary");
        }
        return hashTable.Get(key);
    }


};

#endif // HASHTABLEDICTIONARY_H
