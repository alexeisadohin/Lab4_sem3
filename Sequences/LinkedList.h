#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "ShrdPtr.h"
#include <stdexcept>

template <typename T>
class Node {
public:
    T data;
    ShrdPtr<Node<T>> next;

    Node(const T& data) : data(data) {}
};

template <typename T>
class LinkedList {
private:
    ShrdPtr<Node<T>> head;
    int length;

public:
    LinkedList() :  length(0) {}

    LinkedList(const LinkedList& list) : LinkedList() {
        ShrdPtr<Node<T>> current = list.head;
        while (current) {
            Append(current->data);
            current = current->next;
        }
    }

    LinkedList(const T* items, int count)  : length(0) {
        for (int i = 0; i < count; ++i) {
            Append(items[i]);
        }
    }

    LinkedList(LinkedList&& other) noexcept : head(std::move(other.head)), length(other.length) {
        other.length = 0;
    }

    ~LinkedList() = default;

    T GetFirst() const {
        if (length == 0) {
            throw std::out_of_range("IndexOutOfRange");
        }
        return head->data;
    }

    T GetLast() const {
        if (length == 0) {
            throw std::out_of_range("IndexOutOfRange");
        }
        ShrdPtr<Node<T>> current = head;
        while (current && current->next) {
            current = current->next;
        }
        return current->data;
    }

    const T& Get(int index) const {
        if (index < 0 || index >= length) {
            throw std::out_of_range("IndexOutOfRange");
        }
        ShrdPtr<Node<T>> current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    T& Get(int index)  {
        if (index < 0 || index >= length) {
            throw std::out_of_range("IndexOutOfRange");
        }
        ShrdPtr<Node<T>> current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    ShrdPtr<LinkedList<T>> GetSubList(int startIndex, int endIndex) const {
        if (startIndex < 0 || startIndex >= length || endIndex < 0 || endIndex >= length) {
            throw std::out_of_range("IndexOutOfRange");
        }
        ShrdPtr<LinkedList<T>> sublist(new LinkedList<T>());
        ShrdPtr<Node<T>> current = head;
        for (int i = 0; i < endIndex; ++i) {
            if (i >= startIndex) {
                sublist->Append(current->data);
            }
            current = current->next;
        }
        return sublist;
    }

    int GetLength() const {
        return length;
    }

    void Append(const T& item) {
        ShrdPtr<Node<T>> newNode(new Node<T>(item));
        if (!head) {
            head = newNode;
        } else {
            ShrdPtr<Node<T>> current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
        length++;
    }

    void Prepend(const T& item) {
        ShrdPtr<Node<T>> newNode(new Node<T>(item));
        if (head) {
            newNode->next = head;
        }
        head = newNode;
        length++;
    }

    void InsertAt(const T& item, int index) {
        if (index < 0 || index > length) {
            throw std::out_of_range("IndexOutOfRange");
        }
        if (index == 0) {
            Prepend(item);
            return;
        }
        ShrdPtr<Node<T>> current = head;
        for (int i = 0; i < index - 1; ++i) {
            current = current->next;
        }
        ShrdPtr<Node<T>> newNode(new Node<T>(item));
        newNode->next = current->next;
        current->next = newNode;
        length++;
    }
    void Set(int index, const T& value) {
        if (index < 0 || index >= length) {
            throw std::out_of_range("IndexOutOfRange");
        }
        ShrdPtr<Node<T>> current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        current->data = value;
    }
};

#endif //LINKEDLIST_H
