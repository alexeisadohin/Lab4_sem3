#ifndef IDICTIONARY_H
#define IDICTIONARY_H

#include "ArraySequence.h"

template <typename TKey, typename TElement>
class IDictionary {
public:
    virtual ~IDictionary() = default;
    virtual size_t GetCount() const = 0;
    virtual size_t GetCapacity() const = 0;
    // virtual TElement Get(const TKey& key) const = 0;
    virtual TElement& Get(const TKey& key) const = 0;
    virtual void Add(const TKey& key, const TElement& element) = 0;
    virtual void Remove(const TKey& key) = 0;
    virtual bool ContainsKey(const TKey& key) const = 0;
    virtual ShrdPtr<Sequence<std::pair<TKey, TElement>>> GetAllItems() const = 0;
    class Iterator {
    public:
        virtual std::pair<TKey, TElement> operator*() const = 0;
        virtual Iterator& operator++() = 0;
        virtual bool operator==(const Iterator& other) const = 0;
        virtual bool operator!=(const Iterator& other) const = 0;
        virtual ~Iterator() = default;
    };


    virtual ShrdPtr<Iterator> begin() = 0;
    virtual ShrdPtr<Iterator> end() = 0;

};
#endif //IDICTIONARY_H
