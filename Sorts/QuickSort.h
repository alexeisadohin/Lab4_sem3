#ifndef QUICKSORT_H
#define QUICKSORT_H
#include "ISorter.h"
#include <iostream>
template <typename T, typename Comparator>
class QuickSort : public ISorter<T, Comparator> {
public:
    void Sort(ShrdPtr<ArraySequence<T>> sequence, Comparator comp) {
        size_t n = sequence->GetLength();
        if (n > 0) {
            quickSort(sequence, 0, n - 1, comp);
        }
    }

private:
    size_t partition(ShrdPtr<ArraySequence<T>> sequence, size_t low, size_t high, Comparator comp) {
        T pivot = sequence->Get(high);
        size_t i = (low - 1);

        for (size_t j = low; j <= high - 1; j++) {
            if (comp(sequence->Get(j), pivot)) {
                i++;
                std::swap(sequence->Get(i), sequence->Get(j));
            }
        }
        std::swap(sequence->Get(i + 1), sequence->Get(high));
        return (i + 1);
    }

    void quickSort(ShrdPtr<ArraySequence<T>> sequence, size_t low, size_t high, Comparator comp) {
        if (low < high) {
            size_t pi = partition(sequence, low, high, comp);

            quickSort(sequence, low, pi - 1, comp);
            quickSort(sequence, pi + 1, high, comp);
        }
    }
};

#endif //QUICKSORT_H