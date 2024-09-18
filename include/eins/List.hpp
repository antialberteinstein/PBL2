#ifndef LIST_HPP
#define LIST_HPP

#define LIST_MAX 30

#include <iostream>
#include <string>
#include "algorithm.hpp"

template <class T>
class List {
        T* arr;
        int size;
        int capacity;
    public:
        List(int capacity=LIST_MAX);
        ~List();

        /* Insert an element at the given index. */
        void insert(T element, int index=-1);

        /* Remove the element at the given index. */
        void remove(int index);

        /* Access the element at the given index. */
        T& operator[](int index);

        /* Get the number of elements in the list. */
        int get_size() const;
};

template <class T>
void List<T>::remove(int index) {
    if (index < 0 || index >= size) {
        return;
    }
    for (int i = index; i < size - 1; i++) {
        arr[i] = arr[i + 1];
    }
    size--;
}

template <class T>
T& List<T>::operator[](int index) {
    return arr[index];
}

template <class T>
int List<T>::get_size() const {
    return size;
}



#endif // LIST_HPP