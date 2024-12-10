#ifndef SHRDPTR_H
#define SHRDPTR_H

#include "WeekPtr.h"

#include <set>

template <typename T>
class ShrdPtr {
private:
    struct RefCounter {
        size_t count;
        std::set<WeekPtr<T>*> weekPtrs;
        RefCounter() : count(1) {}
    };

    T* ptr;
    RefCounter* refCounter;

public:
    ShrdPtr() : ptr(nullptr), refCounter(nullptr) {}

    explicit ShrdPtr(T* p) : ptr(p), refCounter(p ? new RefCounter() : nullptr) {}

    ShrdPtr(const ShrdPtr& other) : ptr(other.ptr), refCounter(other.refCounter) {
        if (refCounter) {
            ++refCounter->count;
        }
    }

    ShrdPtr(const WeekPtr<T>& other) : ptr(other.ptr), refCounter(other.refCounter) {
        if (refCounter) {
            ++refCounter->count;
        }
    }

    ShrdPtr(ShrdPtr&& other) noexcept : ptr(other.ptr), refCounter(other.refCounter) {
        other.ptr = nullptr;
        other.refCounter = nullptr;
    }

    ~ShrdPtr() {
        if (refCounter && --refCounter->count == 0) {
            for (auto& weekPtr : refCounter->weekPtrs) {
                weekPtr->invalidate();
            }
            delete ptr;
            delete refCounter;
        }
    }

    ShrdPtr& operator=(const ShrdPtr& other) {
        if (this != &other) {
            if (refCounter && --refCounter->count == 0) {
                delete ptr;
                delete refCounter;
            }
            ptr = other.ptr;
            refCounter = other.refCounter;
            if (refCounter) {
                ++refCounter->count;
            }
        }
        return *this;
    }

    ShrdPtr& operator=(ShrdPtr&& other) noexcept {
        if (this != &other) {
            if (refCounter && --refCounter->count == 0) {
                delete ptr;
                delete refCounter;
            }
            ptr = other.ptr;
            refCounter = other.refCounter;
            other.ptr = nullptr;
            other.refCounter = nullptr;
        }
        return *this;
    }

    T& operator*() {
        if (!ptr) {
            throw std::runtime_error("Trying to dereference a null ShrdPtr");
        }
        return *ptr;
    }

    const T& operator*() const {
        if (!ptr) {
            throw std::runtime_error("Trying to dereference a null ShrdPtr");
        }
        return *ptr;
    }

    T* operator->() {
        if (!ptr) {
            throw std::runtime_error("Trying to dereference a null ShrdPtr");
        }
        return ptr;
    }

    const T* operator->() const {
        if (!ptr) {
            throw std::runtime_error("Trying to dereference a null ShrdPtr");
        }
        return ptr;
    }

    T* Get() const { return ptr; }
    bool operator!() const {
        return ptr == nullptr;
    }
    bool operator!=(std::nullptr_t) const {
        return ptr != nullptr;
    }

    void reset() {
        if (refCounter && --refCounter->count == 0) {
            delete ptr;
            delete refCounter;
        }
        ptr = nullptr;
        refCounter = nullptr;
    }

    void reset(T* newPtr) {
        if (refCounter && --refCounter->count == 0) {
            delete ptr;
            delete refCounter;
        }
        ptr = newPtr;
        refCounter = newPtr ? new RefCounter() : nullptr;
    }

    RefCounter* getRefCounter() const { return refCounter; }

    size_t getRefCount() const {
        return refCounter ? refCounter->count : 0;
    }

    operator bool() const {
        return ptr != nullptr;
    }

    friend class WeekPtr<T>;
};
#endif //SHRDPTR_H