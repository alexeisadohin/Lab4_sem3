#ifndef SEQUENCE_H
#define SEQUENCE_H
#include "UnqPtr.h"
#include "ShrdPtr.h"

template <typename T>
class Sequence {
public:
    virtual ShrdPtr<Sequence<T>>  Append(const T& item) const = 0;
    virtual ShrdPtr<Sequence<T>> Prepend(const T& item) const = 0;
    virtual ShrdPtr<Sequence<T>> GetSubsequence(size_t startIndex, size_t endIndex) const  = 0;
    virtual ShrdPtr<Sequence<T>> InsertAt(const T& item, size_t index) const = 0;
    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual const T& Get(size_t index) const = 0;
    virtual T& Get(size_t index) = 0;
    virtual size_t GetLength() const = 0;
    virtual ShrdPtr<Sequence<T>> Copy() const = 0;
    virtual void RemoveAt(size_t index) = 0;
    virtual ~Sequence() = default;
    virtual void Set(size_t index, const T& value) = 0;
    virtual T& operator[](size_t index) = 0;
    virtual const T& operator[](size_t index) const = 0;
};

#endif //SEQUENCE_H
