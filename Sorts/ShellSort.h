#ifndef SHELLSORT_H
#define SHELLSORT_H
#include "ISorter.h"
template <typename T, typename Comparator>
class ShellSort : public ISorter<T, Comparator> {
public:
    void Sort(ShrdPtr<Sequence<T>> sequence, Comparator comp) override {
        size_t n = sequence->GetLength();
        for (int gap = n / 2; gap > 0; gap /= 2) {
            for (int i = gap; i < n; i++) {
                T temp = sequence->Get(i);
                int j;
                for (j = i; j >= gap && comp(temp, sequence->Get(j - gap)); j -= gap) {
                    sequence->Set(j, sequence->Get(j - gap));
                }
                sequence->Set(j, temp);
            }
        }
    }
};
#endif //SHELLSORT_H
