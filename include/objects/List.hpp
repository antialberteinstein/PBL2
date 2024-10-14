#ifndef PBL_LIST_HPP
#define PBL_LIST_HPP

#include <memory>

using namespace std;

#define DEFAULT_LIST_CAPACITY 10

template <typename T>
class List {
    public:
        List(int capacity=DEFAULT_LIST_CAPACITY);
        List(const List& l);
        
        List& operator=(const List& l);
        List& operator=(const T* arr);

        void add(T item);
        void remove(int index);
        void clear();
        size_t size();

        T& operator[](int index);
    private:
        unique_ptr<T[]> list_;
        size_t size_;
        size_t capacity_;
};

#include "objects/List.tpp"

#endif