 /* My built-in data structures of vectors:
* 1. Vector
*    1.1. How will my vector data structure be built?
*      +) I will use a dynamic array to store the elements.
*      +) I will use a size variable to keep track of the number of elements in the vector.
*      +) I will use a capacity variable to keep track of the maximum number of elements
*           the vector can store, the capacity will have a default value of MAX = 1000.
*       If the number of elements in the vector exceeds the capacity, I will double the capacity.
*      +) I will use a dynamic array to store the index of the elements in the vector.
*   1.2. What operations will my vector data structure support?
*     +) I will support the following operations:
*      - push: to add an element to the end of the vector.
*      - remove(index): to remove an element at the given index.
*      - operator[]: to access an element at the given index.
*      - size: to get the number of elements in the vector.
*      - operator+: to concatenate two vectors.
*      - operator>>: to read the vector from the input stream.
*      - operator<<: to print the vector for debugging purposes.
*      - operator==: to compare two vectors, two vector are equal if they have all the same elements.
*      - operator!=: to compare two vectors, two vector are not equal if they have any different elements.
*      - operator<: to compare two vectors,
*           a vector is less than another vector if from left to right, the first different element is less.
*      - operator>: to compare two vectors,
*           a vector is greater than another vector if from left to right, the first different element is greater.
*      - operator<=: to compare two vectors, < or ==.
*      - operator>=: to compare two vectors, > or ==.
*      - operator=: to assign a vector to another vector.
*/


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