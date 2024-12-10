#ifndef LISTSEQUENCE_H
#define LISTSEQUENCE_H

#include "Sequence.h"
#include "LinkedList.h"
#include "ShrdPtr.h"

template <typename T>
class ListSequence : public Sequence<T> {
private:
    ShrdPtr<LinkedList<T>> list;

public:
    ListSequence() : list(new LinkedList<T>()) {}
    ListSequence(const T* items, int count) : list(new LinkedList<T>(items, count)) {}
    ListSequence(const ListSequence<T>& listSequence) : list(new LinkedList<T>(*listSequence.list)) {}
    ListSequence(const ShrdPtr<LinkedList<T>>& otherList) : list(otherList) {}

    ListSequence(ShrdPtr<LinkedList<T>>&& otherList) : list(std::move(otherList)) {}
    ~ListSequence() override = default;

    ShrdPtr<Sequence<T>> Append(const T& item) const override {
        ShrdPtr<LinkedList<T>> newList = ShrdPtr<LinkedList<T>>(new LinkedList<T>(*this->list));
        newList->Append(item);
        return ShrdPtr<Sequence<T>>(new ListSequence<T>(newList));
    }

    ShrdPtr<Sequence<T>> Prepend(const T& item) const override {
        ShrdPtr<LinkedList<T>> newList =  ShrdPtr<LinkedList<T>>(new LinkedList<T>(*this->list));
        newList->Prepend(item);
        return ShrdPtr<Sequence<T>>(new ListSequence<T>(std::move(newList)));
    }

    ShrdPtr<Sequence<T>> GetSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || endIndex < 0 ||
            startIndex >= list->GetLength() || endIndex >= list->GetLength() ||
            startIndex > endIndex) {
            throw std::out_of_range("Invalid index range for GetSubsequence");
        }

        ShrdPtr<LinkedList<T>> subList = list->GetSubList(startIndex, endIndex);
        ShrdPtr<LinkedList<T>> newList = ShrdPtr<LinkedList<T>>(new LinkedList<T>(*subList));
        return ShrdPtr<Sequence<T>>(new ListSequence<T>(std::move(newList)));
    }

    ShrdPtr<Sequence<T>> InsertAt(const T& item, int index) const override {
        ShrdPtr<LinkedList<T>> newList =  ShrdPtr<LinkedList<T>>(new LinkedList<T>(*this->list));
        newList->InsertAt(item,index);
        return ShrdPtr<Sequence<T>>(new ListSequence<T>(std::move(newList)));
    }

    T GetFirst() const override {
        if (list->GetLength() == 0) {
            throw std::out_of_range("IndexOutOfRange");
        }
        return list->GetFirst();
    }

    T GetLast() const override {
        if (list->GetLength() == 0) {
            throw std::out_of_range("IndexOutOfRange");
        }
        return list->GetLast();
    }

    const T& Get(int index) const override {
        return list->Get(index);
    }
    T& Get(int index) override {
        return list->Get(index);
    }

    int GetLength() const override {
        return list->GetLength();
    }
    ShrdPtr<Sequence<T>> Copy() const override {
        return ShrdPtr<Sequence<T>>(new ListSequence<T>(this->list));
    }
    void Set(int index, const T& value) override {
        list->Set(index, value);
    }
};

#endif //LISTSEQUENCE_H