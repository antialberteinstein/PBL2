#ifndef PBL_STRING_LIST_HPP
#define PBL_STRING_LIST_HPP

#include <iostream>

using namespace std;

class StringList {
    private:
        string* list;
        int size;
        int capacity;
    public:
        StringList(int capacity=10);
        StringList(const StringList& sl);
        StringList(const string* arr, int size);
        ~StringList();
        void add(string item);
        void remove(int index);
        int get_size();
        int get_capacity();
        void clear();

        string& operator[](int index);
};

#endif