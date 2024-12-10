#ifndef WEEKPTR_H
#define WEEKPTR_H

template <typename T>
class ShrdPtr;

template <typename T>
class WeekPtr {
private:
    T* ptr;
    typename ShrdPtr<T>::RefCounter* refCounter;

public:
    WeekPtr() : ptr(nullptr), refCounter(nullptr) {}

    WeekPtr(const ShrdPtr<T>& other) : ptr(other.Get()), refCounter(other.getRefCounter()) {
        if (refCounter) {
            refCounter->weekPtrs.insert(this);
        }
    }

    WeekPtr& operator=(const WeekPtr& other) {
        if (this != &other) {
            if (refCounter) {
                refCounter->weekPtrs.erase(this);
            }
            ptr = other.ptr;
            refCounter = other.refCounter;
            if (refCounter) {
                refCounter->weekPtrs.insert(this);
            }
        }
        return *this;
    }

    WeekPtr(WeekPtr&& other) noexcept : ptr(other.ptr), refCounter(other.refCounter) {
        if (refCounter) {
            refCounter->weekPtrs.erase(&other);
            refCounter->weekPtrs.insert(this);
        }
        other.ptr = nullptr;
        other.refCounter = nullptr;
    }

    WeekPtr& operator=(WeekPtr&& other) noexcept {
        if (this != &other) {
            if (refCounter) {
                refCounter->weekPtrs.erase(this);
            }
            ptr = other.ptr;
            refCounter = other.refCounter;
            if (refCounter) {
                refCounter->weekPtrs.erase(&other);
                refCounter->weekPtrs.insert(this);
            }
            other.ptr = nullptr;
            other.refCounter = nullptr;
        }
        return *this;
    }

    ~WeekPtr() {
        if (refCounter) {
            refCounter->weekPtrs.erase(this);
        }
    }

    T& operator*() {
        if (!ptr) {
            throw std::runtime_error("Trying to dereference a null WeekPtr");
        }
        return *ptr;
    }

    const T& operator*() const {
        if (!ptr) {
            throw std::runtime_error("Trying to dereference a null WeekPtr");
        }
        return *ptr;
    }

    T* operator->() {
        if (!ptr) {
            throw std::runtime_error("Trying to dereference a null WeekPtr");
        }
        return ptr;
    }

    const T* operator->() const {
        if (!ptr) {
            throw std::runtime_error("Trying to dereference a null WeekPtr");
        }
        return ptr;
    }

    bool expired() const { return !refCounter || refCounter->count == 0; }

    ShrdPtr<T> lock() const {
        if (expired()) {
            return ShrdPtr<T>();
        } else {
            return ShrdPtr<T>(*this);
        }
    }

    void invalidate() {
        if(!ptr) {
            throw std::runtime_error("Trying to dereference a null WeekPtr");
        }
        ptr = nullptr;
        refCounter = nullptr;
    }

    friend class ShrdPtr<T>;
};
#endif // WEEKPTR_H