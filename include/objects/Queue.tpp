#include "Queue.hpp"

template <typename T>
Queue<T>::Queue(int capacity) {
    data_ = make_unique<T[]>(capacity);
    front_ = 0;
    rear_ = 0;
    this->capacity_ = capacity;
}

template <typename T>
Queue<T>::Queue(const Queue& l) {
    data_  = make_unique<T[]>(l.capacity_);
    front_ = l.front_;
    rear_ = l.rear_;
    for (size_t i = front_; i != rear_; increment(i)) {
        data_[i] = l.data_[i];
    }
}

template <typename T>
void Queue<T>::enqueue(T item) {
    if (is_full()) {
        expand();
    }
    data_[rear_] = item;
    increment(rear_);
}

template <typename T>
T Queue<T>::dequeue() {
    if (is_empty()) {
        throw queue_empty_exception();
    }
    T item = data_[front_];
    increment(front_);
    return item;
}
