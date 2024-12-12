#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include "ArraySequence.h"
#include <stdexcept>
#include <algorithm>

template <typename T, typename K, typename Compare = std::less<K>>
class PriorityQueue {
private:
    struct Node {
        T item;    // Элемент очереди (например, вершина или ребро)
        K priority; // Приоритет (например, вес)

        Node() : item(T()), priority(K()) {} // Инициализация по умолчанию
        Node(const T& newItem, const K& newPriority) : item(newItem), priority(newPriority) {}
    };

    ArraySequence<Node> sequence;
    Compare comp;

    void HeapifyUp(int index) {
        while (index > 0) {
            int parentIndex = (index - 1) / 2;
            if (!comp(sequence.Get(index).priority, sequence.Get(parentIndex).priority)) break;
            std::swap(sequence.Get(index), sequence.Get(parentIndex));
            index = parentIndex;
        }
    }

    void HeapifyDown(int index) {
        int size = sequence.GetLength();
        while (true) {
            int leftChild = 2 * index + 1;
            int rightChild = 2 * index + 2;
            int smallest = index;

            if (leftChild < size && comp(sequence.Get(leftChild).priority, sequence.Get(smallest).priority)) {
                smallest = leftChild;
            }
            if (rightChild < size && comp(sequence.Get(rightChild).priority, sequence.Get(smallest).priority)) {
                smallest = rightChild;
            }
            if (smallest == index) break;
            std::swap(sequence.Get(index), sequence.Get(smallest));
            index = smallest;
        }
    }

    size_t Parent(size_t i) const { return (i - 1) / 2; }
    size_t LeftChild(size_t i) const { return 2 * i + 1; }
    size_t RightChild(size_t i) const { return 2 * i + 2; }

public:
    PriorityQueue() {}

    void Enqueue(const T& item, const K& priority) {
        sequence = sequence.Append(Node(item, priority));
        HeapifyUp(sequence.GetLength() - 1);
    }

    T Dequeue() {
        if (sequence.GetLength() == 0) {
            throw std::out_of_range("PriorityQueue is empty");
        }
        T first = sequence.Get(0).item;
        std::swap(sequence.Get(0), sequence.Get(sequence.GetLength() - 1));
        sequence.RemoveAt(sequence.GetLength() - 1);
        HeapifyDown(0);
        return first;
    }

    T Peek() const {
        if (sequence.GetLength() == 0) {
            throw std::out_of_range("PriorityQueue is empty");
        }
        return sequence.Get(0).item;
    }

    bool IsEmpty() const {
        return sequence.GetLength() == 0;
    }

    int GetLength() const {
        return sequence.GetLength();
    }

    ~PriorityQueue() = default;
};

#endif // PRIORITYQUEUE_H
