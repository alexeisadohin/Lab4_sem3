#ifndef HEAPSORT_H
#define HEAPSORT_H
#include "ISorter.h"
template <typename T, typename Comparator>
class HeapSort : public ISorter<T, Comparator> {
public:
    void Sort(ShrdPtr<Sequence<T>> sequence, Comparator comp) override {
        size_t n = sequence->GetLength();
        for (int i = n / 2 - 1; i >= 0; --i) {
            heapify(sequence, n, i, comp);
        }
        for (int i = n - 1; i > 0; --i) {
            std::swap(sequence->Get(0), sequence->Get(i));
            heapify(sequence, i, 0, comp);
        }
    }

private:
    void heapify(ShrdPtr<Sequence<T>> sequence, size_t n, size_t i, Comparator comp) {
        size_t largest = i;
        size_t l = 2 * i + 1;
        size_t r = 2 * i + 2;
        if (l < n && comp(sequence->Get(largest), sequence->Get(l))) {
            largest = l;
        }
        if (r < n && comp(sequence->Get(largest), sequence->Get(r))) {
            largest = r;
        }
        if (largest != i) {
            std::swap(sequence->Get(i), sequence->Get(largest));
            heapify(sequence, n, largest, comp);
        }
    }
};
#endif //HEAPSORT_H
