#include "apps/StudentList.hh"

StudentList::~StudentList() {
    // Do nothing
}

StudentList::StudentList() {
    return_btn = Button("Quay lại", [] {
        // Do later/
    });
}