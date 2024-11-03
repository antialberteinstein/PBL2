#ifndef PBL_STUDENT_LIST_HPP
#define PBL_STUDENT_LIST_HPP

#include "apps/App.hh"
#include "objects/Vector.hpp"
#include "objects/Date.hpp"
#include "objects/StringAdapter.hpp"
#include "apps/Scroller.hpp"

void debug(const string& s);

class ModelProducer;

class StudentList : public App {
    public:
        StudentList();
        virtual ~StudentList() = default;
        virtual Element create_element() override;
        virtual bool event(Event event) override;
    private:
        Scroller scroller;
        string error_message;
        bool will_render;  // If false, render error message only
        ModelProducer* student_db;
        Component search_com;
        string search_string;
        string backup_search_string;
        Component info_btn, cancel_btn;
        Component event_listener;
};

#endif // PBL_STUDENT_LIST_HPP