#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include <stdexcept>

template <typename T>
class DynamicArray {
private:
    T* items;
    size_t size;

public:

    DynamicArray(size_t size) : size(size), items(new T[size]) {
        for (size_t i = 0; i < size; ++i) {
            items[i] = T();
        }
    }


    DynamicArray(T* items, size_t count) : size(count), items(new T[count]) {
        for (size_t i = 0; i < count; ++i) {
            this->items[i] = items[i];
        }
    }

    DynamicArray(const DynamicArray<T>& dynamicArray) : size(dynamicArray.size), items(new T[dynamicArray.size]) {
        for (size_t i = 0; i < size; ++i) {
            items[i] = dynamicArray.items[i];
        }
    }

    ~DynamicArray() {
        delete[] items;
    }


    size_t GetSize() const {
        return size;
    }

    const T& Get(size_t index) const {
        if (index < 0 || index >= size) throw std::out_of_range("out_of_range");
        return items[index];
    }

    T& Get(size_t index) {
        if (index < 0 || index >= size) throw std::out_of_range("out_of_range");
        return items[index];
    }

    void Set(size_t index, T value) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("IndexOutOfRange");
        }
        items[index] = value;
    }

    void Resize(size_t newSize) {
        if (newSize <= 0) {
            throw std::length_error("Invalid new size");
        }
        if (newSize == size) {
            return;
        }
        T* newItems = new T[newSize];
        size_t minSize = (newSize > size) ? size : newSize;
        for (size_t i = 0; i < minSize; ++i) {
            newItems[i] = items[i];
        }
        delete[] items;
        items = newItems;
        size = newSize;
    }

    T &operator [] (size_t index) {
        if (index < 0 || index >= size)
            throw std::out_of_range("IndexOutOfRange");
        return items[index];
    }
};

#endif // DYNAMICARRAY_H
