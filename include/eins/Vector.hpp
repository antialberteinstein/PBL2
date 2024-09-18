#ifndef VECTOR_HPP
#define VECTOR_HPP

#define VECTOR_MAX 1000
#include <iostream>
#include <string>
#include "algorithm.hpp"

template <class T>
class Vector {
private:
    T* arr;
    int* indexes;
    int size;
    int capacity;
public:
    Vector(int capacity=VECTOR_MAX);
    ~Vector();

    /* Add an element to the end of the vector. */
    void push(T element);

    /* Remove the element at the given index. */
    void remove(int index);

    /* Access the element at the given index. */
    T& operator[](int index);

    /* Get the number of elements in the vector. */
    int get_size() const;

    /* Concatenate two vectors. */
    Vector<T> operator+(Vector<T> other);

    /* Compare two vectors, two vectors are equal if they have all the same elements. */
    bool operator==(Vector<T> other);

    /* Compare two vectors, two vectors are not equal if they have any different elements. */
    bool operator!=(Vector<T> other);

    /* Compare two vectors, a vector is less than another vector if from left to right,
        the first different element is less. */
    bool operator<(Vector<T> other);

    /* Compare two vectors, a vector is greater than another vector if from left to right,
        the first different element is greater. */
    bool operator>(Vector<T> other);

    /* Compare two vectors, < or ==. */
    bool operator<=(Vector<T> other);

    /* Compare two vectors, > or ==. */
    bool operator>=(Vector<T> other);

    /* Assign a vector to another vector. */
    Vector<T>& operator=(Vector<T> other);

    /* Show the vector */
    void show();

    /* Convert to array */
    T* to_array();
};

#endif // VECTOR_HPP