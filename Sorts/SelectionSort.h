#ifndef SELECTIONSORT_H
#define SELECTIONSORT_H
#include "ISorter.h"
template <typename T, typename Comparator>
class SelectionSort : public ISorter<T, Comparator> {
public:
    void Sort(ShrdPtr<Sequence<T>> sequence, Comparator comp) override {
        size_t n = sequence->GetLength();
        for (size_t i = 0; i < n; ++i) {
            size_t min_idx = i;
            for (size_t j = i + 1; j < n; ++j) {
                if (comp(sequence->Get(j), sequence->Get(min_idx))) {
                    min_idx = j;
                }
            }
            std::swap(sequence->Get(min_idx), sequence->Get(i));
        }
    }
};

#endif //SELECTIONSORT_H
