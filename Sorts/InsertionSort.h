#ifndef INSERTIONSORT_H
#define INSERTIONSORT_H
#include "ISorter.h"

template <typename T, typename Comparator>
class InsertionSort : public ISorter<T, Comparator> {
public:
    void Sort(ShrdPtr<Sequence<T>> sequence, Comparator comp) override {
        size_t n = sequence->GetLength();
        for (size_t i = 1; i < n; ++i) {
            T key = sequence->Get(i);
            int j = i - 1;
            while (j >= 0 && comp(key, sequence->Get(j))) {
                sequence->Set(j + 1, sequence->Get(j));
                --j;
            }
            sequence->Set(j + 1, key);
        }
    }
};
#endif //INSERTIONSORT_H
