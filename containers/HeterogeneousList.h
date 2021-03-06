//
// Created by zdravko on 22.01.16.
//

#ifndef CONTAINERS_HETEROGENEOUSLIST_H
#define CONTAINERS_HETEROGENEOUSLIST_H

#include <list>
#include <fstream>
#include <string>
#include <sstream>
#include <queue>
#include "HeterogeneousListContainer.h"

using std::list;
using std::ifstream;
using std::ofstream;
using std::string;
using std::istringstream;
using std::vector;
using std::priority_queue;

template <class T>
class HeterogeneousListIterator;
template <class T>
class SortedHeterogeneousListIterator;

template <class T>
class HeterogeneousList {
    template<class P>
    friend class HeterogeneousListIterator;
    template<class P>
    friend class SortedHeterogeneousListIterator;

    list<HeterogeneousListContainer<T>*> containers;

    class ContainerSizeComparer {
    public:
        bool operator() (HeterogeneousListContainer<T> *first, HeterogeneousListContainer<T> *second) {
            return first->size() > second->size();
        }
    };
    priority_queue<HeterogeneousListContainer<T>*, vector<HeterogeneousListContainer<T>*>, ContainerSizeComparer> smallestFirstContainers;

    using Condition = bool (*)(T const&);

    void clean() {
        for (auto it = containers.begin(); it != containers.end(); ++it)
            delete *it;
    }

    void copy(const HeterogeneousList<T> &other) {
        containers = list<HeterogeneousListContainer<T>*>();
        smallestFirstContainers = priority_queue<HeterogeneousListContainer<T>*, vector<HeterogeneousListContainer<T>*>, ContainerSizeComparer>();
        for (auto it = other.containers.begin(); it != other.containers.end(); ++it) {
            auto containerClone = (*it)->clone();
            containers.push_back(containerClone);
            smallestFirstContainers.push(containerClone);
        }
    }

public:
    HeterogeneousList(ifstream &input) {
        int containerType, tempNumber;
        string lineString;
        HeterogeneousListContainer<T> *container;
        while (getline(input, lineString)) {
            istringstream lineStream(lineString);
            lineStream >> containerType;
            switch (containerType) {
                case 0:
                    container = new DoublyLinkedList<T>();
                    break;
                case 1:
                    container = new Queue<T>();
                    break;
                case 2:
                    container = new Stack<T>();
                    break;
            }
            while (lineStream >> tempNumber)
                container->addItem(tempNumber);

            containers.push_back(container);
            smallestFirstContainers.push(container);
        }
    }

    HeterogeneousList(const HeterogeneousList &other) {
        copy(other);
    }

    HeterogeneousList &operator=(const HeterogeneousList &other) {
        if (this != &other) {
            clean();
            copy(other);
        }

        return *this;
    }

    ~HeterogeneousList() {
        clean();
    }

    void addItemLoadBalanced(const T item) {
        HeterogeneousListContainer<T> *smallestContainer = smallestFirstContainers.top();
        smallestFirstContainers.pop();
        smallestContainer->addItem(item);
        smallestFirstContainers.push(smallestContainer);
    }

    bool member(const T &x) {
        for (auto it = containers.begin(); it != containers.end(); ++it) {
            if ((*it)->member(x))
                return true;
        }
        return false;
    }

    bool search(Condition predicate) {
        for (auto it = containers.begin(); it != containers.end(); ++it) {
            if ((*it)->search(predicate))
                return true;
        }
        return false;
    }

    void removeMatching(Condition predicate) {
        for (auto it = containers.begin(); it != containers.end(); ++it)
            (*it)->removeMatching(predicate);
    }

    void print() {
        for (auto it = containers.begin(); it != containers.end(); ++it)
            (*it)->print();
    }

    void sortEachContainer() {
        for (auto it = containers.begin(); it != containers.end(); ++it)
            (*it)->sort();
    }

    SortedHeterogeneousListIterator<T> findInSorted(const T &item) {
        SortedHeterogeneousListIterator<T> it(*this);
        while (it) {
            if (item == *it)
                return it;

            ++it;
        }
        return SortedHeterogeneousListIterator<T>();
    }

    void writeOut(ofstream &output) {
        for (auto it = containers.begin(); it != containers.end(); ++it)
            (*it)->write(output);
    }
};


#endif //CONTAINERS_HETEROGENEOUSLIST_H
