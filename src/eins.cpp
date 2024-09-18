#include "eins/Vector.hpp"
#include "eins/List.hpp"
#include "eins/algorithm.hpp"

namespace algorithm {
    // =================================================================
    //                           ALGORITHM
    // =================================================================

    template <class T>
    void swap(T& a, T& b) {
        T temp = a;
        a = b;
        b = temp;
    }
}

// =================================================================
//                           LIST
// =================================================================

template <class T>
List<T>::List(int capacity) {
    try {
        this->capacity = capacity;
        arr = new T[capacity];
        size = 0;
    } catch (std::bad_alloc& e) {
        delete[] arr;
        throw;
    }
}

template <class T>
List<T>::~List() {
    delete[] arr;
}

template <class T>
void List<T>::insert(T element, int index) {
    if (size == capacity) {
        return;
    }
    if (index = -1) {
        index = size;
    }
    for (int i = size; i > index; i--) {
        arr[i] = arr[i - 1];
    }
    arr[index] = element;
    size++;
}

// =================================================================
//                           VECTOR
// =================================================================


template <class T>
Vector<T>::Vector(int capacity) {
    try {
        this->capacity = capacity;
        arr = new T[capacity];
        indexes = new int[capacity];
        size = 0;
    } catch (std::bad_alloc& e) {
        delete[] arr;
        delete[] indexes;
        throw;
    }
}

template <class T>
Vector<T>::~Vector() {
    delete[] arr;
    delete[] indexes;
}

template <class T>
void Vector<T>::push(T element) {
    if (size == capacity) {
        try {
            capacity *= 2;
            T* new_arr = new T[capacity];
            int* new_indexes = new int[capacity];
            for (int i = 0; i < size; i++) {
                new_arr[i] = arr[i];
                new_indexes[i] = indexes[i];
            }
            delete[] arr;
            delete[] indexes;
            arr = new_arr;
            indexes = new_indexes;
        } catch (std::bad_alloc& e) {
            delete[] arr;
            delete[] indexes;
            throw;
        }
    }
    arr[size] = element;
    indexes[size] = size;
    size++;
}

template <class T>
void Vector<T>::remove(int index) {
    if (index < 0 || index >= size) {
        return;
    }
    algorithm::swap(arr[index], arr[size - 1]);
    algorithm::swap(indexes[index], indexes[size - 1]);
    size--;
}

template <class T>
int Vector<T>::get_size() const {
    return size;
}

template <class T>
Vector<T> Vector<T>::operator+(Vector<T> other) {
    Vector<T> result(size + other.size);
    for (int i = 0; i < size; i++) {
        result.push(arr[i]);
    }
    for (int i = 0; i < other.size; i++) {
        result.push(other.arr[i]);
    }
    return result;
}

template <class T>
bool Vector<T>::operator==(Vector<T> other) {
    if (size != other.size) {
        return false;
    }
    for (int i = 0; i < size; i++) {
        if (arr[i] != other.arr[i]) {
            return false;
        }
    }
    return true;
}

template <class T>
bool Vector<T>::operator!=(Vector<T> other) {
    return !(*this == other);
}

template <class T>
bool Vector<T>::operator<(Vector<T> other) {
    int foo = 0;
    while (foo < size && foo < other.size) {
        if (arr[foo] < other.arr[foo]) {
            return true;
        } else if (arr[foo] > other.arr[foo]) {
            return false;
        }
    }
    if (size < other.size) {
        return true;
    }
    return false;
}

template <class T>
bool Vector<T>::operator<=(Vector<T> other) {
    return *this < other || *this == other;
}

template <class T>
bool Vector<T>::operator>(Vector<T> other) {
    return !(*this <= other);
}

template <class T>
bool Vector<T>::operator>=(Vector<T> other) {
    return !(*this < other);
}

template <class T>
Vector<T>& Vector<T>::operator=(Vector<T> other) {
    delete[] arr;
    delete[] indexes;
    arr = new T[other.capacity];
    indexes = new int[other.capacity];
    size = other.size;
    capacity = other.capacity;
    for (int i = 0; i < size; i++) {
        arr[i] = other.arr[i];
        indexes[i] = other.indexes[i];
    }
    return *this;
}

template <class T>
T& Vector<T>::operator[](int index) {
    return arr[indexes[index]];
}

template <class T>
void Vector<T>::show() {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

template <class T>
T* Vector<T>::to_array() {
    T* result = new T[size];
    for (int i = 0; i < size; i++) {
        result[i] = arr[i];
    }
    return result;
}


#include "tui.h"

template class Vector<string>;
template class List<MenuOption>;