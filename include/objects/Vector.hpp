#ifndef PBL_VECTOR_HPP
#define PBL_VECTOR_HPP


template <typename T>
class Vector {
    public:
        Vector();
        Vector(const Vector<T>& v);
        Vector(T* data);

        Vector& operator=(const Vector<T>& v);
        Vector& operator=(T* data);

        void push_back(const T& value);
        T pop_back();
        T at(size_t index) const;
        T& operator[](size_t index);
        size_t size() const;
        void clear();
    private:
        unique_ptr<T[]> data_;
        size_t size_;
        size_t capacity_;

        void resize();
};

#include "objects/Vector.tpp"

#endif