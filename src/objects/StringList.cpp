#include "objects/StringList.hpp"

StringList::StringList(int capacity) {
    this->capacity = capacity;
    this->size = 0;
    this->list = new string[capacity];
}

StringList::StringList(const StringList& sl) {
    this->capacity = sl.capacity;
    this->size = sl.size;
    this->list = new string[capacity];
    for (int i = 0; i < size; i++) {
        list[i] = sl.list[i];
    }
}

StringList::StringList(const string* arr, int size) {
    this->capacity = size;
    this->size = size;
    this->list = new string[capacity];
    for (int i = 0; i < size; i++) {
        list[i] = arr[i];
    }
}

StringList::~StringList() {
    delete []list;
}

void StringList::add(string item) {
    if (size == capacity) {
        return;
    }
    list[size++] = item;
}

void StringList::remove(int index) {
    if (index < 0 || index >= size) {
        return;
    }
    for (int i = index; i < size - 1; i++) {
        list[i] = list[i + 1];
    }
    size--;
}

string& StringList::operator[](int index) {
    return list[index];
}

int StringList::get_size() {
    return size;
}

int StringList::get_capacity() {
    return capacity;
}

void StringList::clear() {
    size = 0;
}