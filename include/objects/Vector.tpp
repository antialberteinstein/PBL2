#include <objects/Vector.hpp>

template <typename T>
Vector<T>::Vector() : size_(0), capacity_(1) {
    data_ = make_unique<T[]>(capacity_);
}

template <typename T>
void Vector<T>::push_back(const T& value) {
    if (size_ == capacity_) {
        resize();
    }
    data_[size_++] = value;
}

template <typename T>
T Vector<T>::at(size_t index) const {
    return data_[index];
}

template <typename T>
T& Vector<T>::operator[](size_t index) {
    return data_[index];
}

template <typename T>
size_t Vector<T>::size() const {
    return size_;
}

template <typename T>
void Vector<T>::resize() {
    capacity_ *= 2;
    unique_ptr<T[]> new_data = make_unique<T[]>(capacity_);
    for (size_t i = 0; i < size_; i++) {
        new_data[i] = data_[i];
    }
    data_ = move(new_data);
}

template <typename T>
void Vector<T>::clear() {
    size_ = 0;
}

template <typename T>
Vector<T>& Vector<T>::operator=(T* data) {
    size_ = 0;
    while (data[size_] != nullptr) {
        push_back(data[size_]);
    }
    return *this;
}

template <typename T>
Vector<T>::Vector(T* data) : size_(0), capacity_(1) {
    this->data_ = make_unique<T[]>(capacity_);
    while (data[size_] != nullptr) {
        push_back(data[size_]);
    }
}

template <typename T>
Vector<T>::Vector(const Vector<T>& v) : size_(v.size_), capacity_(v.capacity_) {
    data_ = make_unique<T[]>(capacity_);
    for (size_t i = 0; i < size_; i++) {
        data_[i] = v.data_[i];
    }
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& v) {
    size_ = v.size_;
    capacity_ = v.capacity_;
    data_ = make_unique<T[]>(capacity_);
    for (size_t i = 0; i < size_; i++) {
        data_[i] = v.data_[i];
    }
    return *this;
}