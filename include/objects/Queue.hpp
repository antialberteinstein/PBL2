#include <memory>
#include <exception>
using namespace std;

#define DEFAULT_QUEUE_CAPACITY 10

#pragma once

template <typename T>
class Queue {
    public:
        Queue(int capacity = DEFAULT_QUEUE_CAPACITY);
        Queue(const Queue& l);

        void enqueue(T item);
        T dequeue();

        bool is_empty() const {
            return front_ == rear_;
        }

        bool is_full() const {
            return ((rear_ + 1) % capacity_ == front_);
        }

    private:
        unique_ptr<T[]> data_;
        size_t front_;
        size_t rear_;
        size_t capacity_;

        void increment(size_t& s) {
            s = (s + 1) % capacity_;
        }

        void expand() {
            size_t new_capacity = capacity_ * 2;
            auto new_data = make_unique<T[]>(new_capacity);
            size_t new_front = 0;
            size_t new_index = 0;
            for (size_t i = front_; i != rear_; increment(i)) {
                new_data[new_index] = data_[i];
                new_index++;
            }
            data_ = move(new_data);
            front_ = new_front;
            rear_ = new_index;
            capacity_ *= 2;
        }
};

class queue_empty_exception : public exception {
    public:
        const char* what() const noexcept override {
            return "Queue is empty!";
        }
};

#include "Queue.tpp"
