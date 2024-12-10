#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdexcept>
#include <utility>
#include <memory>


template <typename TKey, typename TElement, typename Hash = std::hash<TKey>>
class HashTable {
private:
    struct Entry {
        TKey key;
        TElement element;
        bool occupied;
        bool wasDeleted;
        Entry() : occupied(false), wasDeleted(false) {}
    };

    Entry* table;
    size_t count;
    size_t capacity;
    Hash hash;

    size_t hashKey(const TKey& key) const {
        return hash(key) % capacity;
    }

    void resizeTable(size_t newCapacity) {
        Entry* oldTable = table;
        size_t oldCapacity = capacity;

        capacity = newCapacity;
        count = 0;
        table = new Entry[capacity];

        for (size_t i = 0; i < oldCapacity; ++i) {
            if (oldTable[i].occupied && !oldTable[i].wasDeleted) {
                Add(oldTable[i].key, oldTable[i].element);
            }
        }


        delete[] oldTable;
    }

    size_t findNode(const TKey& key) const {
        size_t index = hashKey(key);
        size_t originalIndex = index;
        size_t deletedIndex = -1; // Индекс удаленного элемента

        while (table[index].occupied || table[index].wasDeleted) {
            if (table[index].wasDeleted && deletedIndex == -1) {
                deletedIndex = index; // Запоминаем первый найденный удаленный элемент
            }
            if (table[index].occupied && table[index].key == key) {
                return index;
            }
            index = (index + 1) % capacity;
            if (index == originalIndex) {
                break;
            }
        }

        return deletedIndex != -1 ? deletedIndex : -1;
    }


public:
    explicit HashTable(size_t initialCapacity = 25)
        : count(0) {

        capacity = initialCapacity > 0 ? initialCapacity : 25;

        table = new Entry[capacity];
    }

    HashTable(const HashTable& other) : count(other.count), capacity(other.capacity), hash(other.hash) {
        table = new Entry[capacity];
        for (size_t i = 0; i < capacity; ++i) {
            table[i] = other.table[i];
        }
    }

    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            delete[] table;
            count = other.count;
            capacity = other.capacity;
            hash = other.hash;
            table = new Entry[capacity];
            for (size_t i = 0; i < capacity; ++i) {
                table[i] = other.table[i];
            }
        }
        return *this;
    }

    ~HashTable() {
        delete[] table;
    }

    size_t GetCount() const { return count; }
    size_t GetCapacity() const { return capacity; }

    // TElement Get(const TKey& key) const {
    //     size_t index = findNode(key);
    //     if (index == -1) {
    //         throw std::runtime_error("Element not found");
    //     }
    //     return table[index].element;
    // }

    TElement& Get(const TKey& key) const{
        size_t index = findNode(key);
        if (index == -1) {
            throw std::runtime_error("Element not found");
        }
        return table[index].element;
    }

    void Add(const TKey& key, const TElement& element) {
        if (count >= capacity * 0.75) {
            resizeTable(capacity * 2);
        }

        size_t index = findNode(key);

        if (index == -1) {
            index = hashKey(key);
            size_t originalIndex = index;
            while (table[index].occupied) {
                index = (index + 1) % capacity;
                if (index == originalIndex) {
                    throw std::runtime_error("Hash table is full");
                }
            }
        }

        table[index].key = key;
        table[index].element = element;
        table[index].occupied = true;
        table[index].wasDeleted = false;
        ++count;
    }

    void Remove(const TKey& key) {
        size_t index = findNode(key);
        if (index == -1) {
            throw std::runtime_error("Element not found");
        }

        if (table[index].occupied) {
            table[index].occupied = false;
            table[index].wasDeleted = true;
            --count;
        }
    }

    void RemoveAll() {
        for (size_t i = 0; i < capacity; ++i) {
            if (table[i].occupied && !table[i].wasDeleted) {
                table[i].occupied = false;
                table[i].wasDeleted = true;
                --count;
            }
        }

    }

    bool ContainsKey(const TKey& key) const {
        return findNode(key) != -1;
    }

    class Iterator {
    private:
        const HashTable* hashTable;
        size_t currentIndex;

        void moveToNextValid() {
            while (currentIndex < hashTable->capacity &&
                   !hashTable->table[currentIndex].occupied) {
                ++currentIndex;
            }
        }

    public:
        Iterator(const HashTable* ht, size_t start)
            : hashTable(ht), currentIndex(start) {
            moveToNextValid();
        }

        Iterator& operator++() {
            ++currentIndex;
            moveToNextValid();
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return hashTable == other.hashTable && currentIndex == other.currentIndex;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        std::pair<TKey, TElement> operator*() const {
            if (currentIndex >= hashTable->capacity) {
                throw std::runtime_error("Iterator out of bounds");
            }
            return {hashTable->table[currentIndex].key, hashTable->table[currentIndex].element};
        }
    };

    Iterator begin() const {
        return Iterator(this, 0);
    }

    Iterator end() const {
        return Iterator(this, capacity);
    }

    ShrdPtr<Sequence<std::pair<TKey, TElement>>> GetAllItems() const {
        auto items = ShrdPtr<Sequence<std::pair<TKey, TElement>>>(new ArraySequence<std::pair<TKey, TElement>>());
        for (const auto& item : *this) {
            items = items->Append(item);
        }
        return items;
    }
};

#endif // HASHTABLE_H
