#ifndef UNQPTR_H
#define UNQPTR_H

#include <stdexcept>
#include <utility>

template <typename T>
class UnqPtr {
private:
    T* ptr;

public:
    UnqPtr() : ptr(nullptr) {}

    explicit UnqPtr(T* p) : ptr(p) {}

    UnqPtr(UnqPtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    ~UnqPtr() {
        delete ptr;
    }

    UnqPtr& operator=(UnqPtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    UnqPtr(const UnqPtr&) = delete;
    UnqPtr& operator=(const UnqPtr&) = delete;

    T& operator*() {
        if (!ptr) {
            throw std::runtime_error("Trying to dereference a null UnqPtr");
        }
        return *ptr;
    }

    const T& operator*() const {
        if (!ptr) {
            throw std::runtime_error("Trying to dereference a null UnqPtr");
        }
        return *ptr;
    }

    T* operator->() {
        if (!ptr) {
            throw std::runtime_error("Trying to dereference a null UnqPtr");
        }
        return ptr;
    }

    const T* operator->() const {
        if (!ptr) {
            throw std::runtime_error("Trying to dereference a null UnqPtr");
        }
        return ptr;
    }

    T* Get() const { return ptr; }

    void swap(UnqPtr& other) noexcept {
        std::swap(ptr, other.ptr);
    }

    bool operator!() const {
        return ptr == nullptr;
    }
    bool operator!=(std::nullptr_t) const {
        return ptr != nullptr;
    }
};
#endif //UNQPTR_H