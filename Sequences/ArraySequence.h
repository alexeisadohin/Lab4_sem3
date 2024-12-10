#ifndef ARRAYSEQUENCE_H
#define ARRAYSEQUENCE_H

#include "Sequence.h"
#include "DynamicArray.h"
#include <iostream>

template <typename T>
class ArraySequence : public Sequence<T> {
private:
    ShrdPtr<DynamicArray<T>> array;

    void EnsureCapacity(size_t requiredCapacity) {
        if (array->GetSize() < requiredCapacity) {
            size_t newCapacity = array->GetSize() * 2 + 1;
            if (newCapacity < requiredCapacity) {
                newCapacity = requiredCapacity;
            }
            array->Resize(newCapacity);
        }
    }
public:
    ArraySequence() : array(new DynamicArray<T>(0)) {}
    ArraySequence(ShrdPtr<DynamicArray<T>>&& arr) : array(std::move(arr)) {}
    ArraySequence(T* items, size_t count) : array(new DynamicArray<T>(items, count)) {}
    ArraySequence(const ArraySequence<T>& arraySequence) : array(new DynamicArray<T>(*arraySequence.array)) {}
    ArraySequence(const ShrdPtr<DynamicArray<T>>& otherArray) : array(otherArray) {}


    ShrdPtr<Sequence<T>>  Append(const T& item) const override {
        ShrdPtr<DynamicArray<T>> newArray(new DynamicArray<T>(this->array->GetSize() + 1));
        for (size_t i = 0; i < this->array->GetSize(); ++i) {
            newArray->Set(i, this->array->Get(i));
        }
        newArray->Set(newArray->GetSize() - 1, item);
        return ShrdPtr<Sequence<T>>(new ArraySequence<T>(newArray));
    }

    ShrdPtr<Sequence<T>> Prepend(const T& item) const override {
        ShrdPtr<DynamicArray<T>> newArray(new DynamicArray<T>(this->array->GetSize() + 1));
        newArray->Set(0, item);
        for (size_t i = 0; i < this->array->GetSize(); ++i) {
            newArray->Set(i + 1, this->array->Get(i));
        }
        return ShrdPtr<Sequence<T>>(new ArraySequence<T>(std::move(newArray)));
    }

    ShrdPtr<Sequence<T>> InsertAt(const T& item, size_t index) const override {
        ShrdPtr<DynamicArray<T>> newArray(new DynamicArray<T>(this->array->GetSize() + 1));
        for (size_t i = 0; i < index; ++i) {
            newArray->Set(i, this->array->Get(i));
        }
        newArray->Set(index, item);
        for (size_t i = index; i < this->array->GetSize(); ++i) {
            newArray->Set(i + 1, this->array->Get(i));
        }
        return ShrdPtr<Sequence<T>>(new ArraySequence<T>(std::move(newArray)));
    }

    ShrdPtr<Sequence<T>> GetSubsequence(size_t startIndex, size_t endIndex) const override {
        if (startIndex < 0 || startIndex >= array->GetSize() || endIndex < 0 || endIndex >= array->GetSize() || startIndex > endIndex) {
            throw std::out_of_range("IndexOutOfRange");
        }
        ShrdPtr<DynamicArray<T>> newArray(new DynamicArray<T>(endIndex - startIndex + 1));
        for (size_t i = startIndex; i <= endIndex; i++) {
            newArray->Set(i - startIndex, array->Get(i));
        }
        return ShrdPtr<Sequence<T>>(new ArraySequence<T>(std::move(newArray)));
    }

    T GetFirst() const override {
        if (array->GetSize() == 0) {
            throw std::out_of_range("IndexOutOfRange");
        }
        return array->Get(0);
    }

    T GetLast() const override {
        if (array->GetSize() == 0) {
            throw std::out_of_range("IndexOutOfRange");
        }
        return array->Get(array->GetSize() - 1);
    }

    const T& Get(size_t index) const override {
        if (index < 0 || index >= array->GetSize()) {
            std::cerr << "Error in get: Index " << index << " out of range [0, " << array->GetSize() - 1 << "]" << std::endl;
            throw std::out_of_range("Index out of range");
        }
        return array->Get(index);
    }
    T& Get(size_t index) override {
        if (index < 0 || index >= array->GetSize()) {
            std::cerr << "Error in get: Index " << index << " out of range [0, " << array->GetSize() - 1 << "]" << std::endl;
            throw std::out_of_range("Index out of range");
        }
        return array->Get(index);
    }

    void Set(size_t index, const T& value) override {
        if (index < 0 || index >= array->GetSize()) {
            std::cerr << "Error in set: Index " << index << " out of range [0, " << array->GetSize() - 1 << "]" << std::endl;
            throw std::out_of_range("Index out of range");
        }
        array->Set(index, value);
    }

    size_t GetLength() const override {
        return array->GetSize();
    }

    ShrdPtr<Sequence<T>> Copy() const override {
        return ShrdPtr<Sequence<T>>(new ArraySequence<T>(this->array));
    }

    T& operator[](size_t index) override {
        return Get(index);
    }

    const T& operator[](size_t index) const override {
        return Get(index);
    }
    void RemoveAt(size_t index) override {
        if (index < 0 || index >= array->GetSize()) {
            throw std::out_of_range("Index out of range");
        }
        ShrdPtr<DynamicArray<T>> newArray(new DynamicArray<T>(array->GetSize() - 1));
        for (size_t i = 0, j = 0; i < array->GetSize(); ++i) {
            if (i != index) {
                newArray->Set(j++, array->Get(i));
            }
        }
        array = newArray;
    }

    ArraySequence<T>& operator=(const ShrdPtr<Sequence<T>>& other) {
        if (this == other.Get()) {
            return *this; // Защита от самоприсваивания
        }
        // Копируем элементы из другого Sequence
        this->array = ShrdPtr<DynamicArray<T>>(new DynamicArray<T>(other->GetLength()));
        for (size_t i = 0; i < other->GetLength(); ++i) {
            this->array->Set(i, other->Get(i));
        }
        return *this;
    }

    bool Contains(const T& item) const {
        for (size_t i = 0; i < this->GetLength(); ++i) {
            if (this->operator[](i) == item) {
                return true;
            }
        }
        return false;
    }

    void Add(const T& item) {
        array->Resize(array->GetSize() + 1);
        array->Set(array->GetSize() - 1, item);
    }

    ~ArraySequence() = default;
};

#endif // ARRAYSEQUENCE_H