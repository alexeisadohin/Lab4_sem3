#ifndef BUBBLESORT_H
#define BUBBLESORT_H

#include "ISorter.h"

template <typename T, typename Comparator>
class BubbleSort: public ISorter<T, Comparator> {
public:
    void Sort(ShrdPtr<Sequence<T>> sequence, Comparator comp) override {
        int n = sequence->GetLength();
        for (int i = 0; i < n-1; i++) {
            for (int j = 0; j < n-i-1; j++){
                if (comp(sequence->Get(j+1),sequence->Get(j))) {
                    std::swap(sequence->Get(j), sequence->Get(j+1));
                }
            }
        }
    }
};

#endif //BUBBLESORT_H
