#ifndef PBL_STUDENT_LIST_HH
#define PBL_STUDENT_LIST_HH

#include "apps/App.hh"
#include "objects/Student.hpp"
#include "objects/List.hpp"
#include "objects/Adapter.hpp"
#include "objects/Vector.hpp"

class StudentList : public App {
    public:
        StudentList();
        virtual ~StudentList();
        virtual Element create_element();
        virtual bool event(Event event);
    protected:
        Component render_com;
        Component menu_list;
        Component return_btn;
        Vector<string> student_names;
        int selected = 0;
        List<Student> students;
        string error_message;
        Vector<string> titles;
};
#endif