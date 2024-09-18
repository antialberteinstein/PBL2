#ifndef PBL_EINS_H
#define PBL_EINS_H

#include <iostream>
#include <string>

namespace data_structures {

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
    //  The vector is defined in the Vector.hpp file.

    //  The list is defined in the List.hpp file.
}

#endif // PBL_EINS_H