#include "objects/List.hpp"


template <typename T>
List<T>::List(int capacity) : size_(0), capacity_(capacity) {
    list_ = make_unique<T[]>(capacity_);
}

template <typename T>
List<T>::List(const List& l) : size_(l.size_), capacity_(l.capacity_) {
    list_ = make_unique<T[]>(capacity_);
    for (size_t i = 0; i < size_; i++) {
        list_[i] = l.list_[i];
    }
}

template <typename T>
List<T>& List<T>::operator=(const List& l) {
    size_ = l.size_;
    capacity_ = l.capacity_;
    list_ = make_unique<T[]>(capacity_);
    for (size_t i = 0; i < size_; i++) {
        list_[i] = l.list_[i];
    }
    return *this;
}

template <typename T>
List<T>& List<T>::operator=(const T* arr) {
    size_ = 0;
    while (arr[size_]) {
        size_++;
    }
    capacity_ = size_ * 2;
    list_ = make_unique<T[]>(capacity_);
    for (size_t i = 0; i < size_; i++) {
        list_[i] = arr[i];
    }
    return *this;
}

template <typename T>
void List<T>::add(T item) {
    if (size_ == capacity_) {
        capacity_ *= 2;
        unique_ptr<T[]> new_list = make_unique<T[]>(capacity_);
        for (size_t i = 0; i < size_; i++) {
            new_list[i] = list_[i];
        }
        list_ = move(new_list);
    }
    list_[size_++] = item;
}

template <typename T>
void List<T>::remove(int index) {
    if (index < 0 || index >= size_) {
        return;
    }
    for (size_t i = index; i < size_ - 1; i++) {
        list_[i] = list_[i + 1];
    }
    size_--;
}

template <typename T>
void List<T>::clear() {
    size_ = 0;
}

template <typename T>
size_t List<T>::size() {
    return size_;
}

template <typename T>
T& List<T>::operator[](int index) {
    return list_[index];
}

