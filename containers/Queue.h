//
// Created by zdravko on 22.01.16.
//

#ifndef CONTAINERS_QUEUE_H
#define CONTAINERS_QUEUE_H

#include "DoublyLinkedList.h"

template <class T>
class Queue : public HeterogeneousListContainer<T> {
    DoublyLinkedList<T> container;

    using Condition = bool (*)(T const&);

public:
    Queue() {
        container = DoublyLinkedList<T>();
    }

    HeterogeneousListContainer<T> *clone() const {
        return new Queue<T>(*this);
    }

    DoublyLinkedListNode<T> *begin() const {
        return container.begin();
    }

    void queue(const T item) {
        container.insertBack(item);
    }

    T front() {
        return container.begin()->data;
    }

    bool empty() {
        return container.empty();
    }

    bool dequeue() {
        if (empty())
            return false;

        container.removeAt(container.begin());
        return true;
    }

    bool member(const T &item) {
        return container.member(item);
    }

    bool search(Condition predicate) {
        return  container.search(predicate);
    }

    void removeMatching(Condition predicate) {
        container.removeMatching(predicate);
    }

    void addItem(const T item) {
        queue(item);
    }

    void removeItem() {
        dequeue();
    }

    void print() {
        std::cout << *this;
    }

    int size() {
        return container.size();
    }

    void sort() {
        container.sort();
    }

    void write(std::ofstream &output) {
        output << "1 ";
        for (auto it = begin(); it != nullptr; it = it->next) {
            output << it->data << ' ';
        }
        output << std::endl;
    }

    template <class P> friend ostream& operator<<(ostream&, const Queue<P>&);
};

template<class T>
ostream& operator<<(ostream& os, const Queue<T> &l) {
    os << "> ";
    for(DoublyLinkedListNode<T> *node = l.container.end(); node != nullptr; node = node->prev)
        os << node->data << " > ";
    return os << std::endl;
}


#endif //CONTAINERS_QUEUE_H
