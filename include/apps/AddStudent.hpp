#ifndef PBL_ADD_STUDENT_HPP
#define PBL_ADD_STUDENT_HPP

#include "apps/App.hh"
#include "objects/Vector.hpp"
#include "objects/Student.hpp"
#include "objects/Date.hpp"

struct EditText {
    string label;
    string value;
    Component com;
};

struct ComboBox {
    string label;
    Vector<string> values;
    int selected;
    Component com;
};

class AddStudent : public App {
    public:
        AddStudent();
        virtual ~AddStudent() = default;
        virtual Element create_element();
        virtual bool event(Event event);
    private:
        Component confirm_btn,
                  cancel_btn,
                  event_listener;
        EditText name, dob, hometown,
                 university, major,
                 phone, email;
        ComboBox gender;
        string error_message;
};

inline Element get_doc(EditText& field) {
    return hbox({
        text(field.label),
        separator(),
        text(INPUT_PADDING),
        field.com->Render() | inverted,
    }) | border;
}

inline Element get_doc(ComboBox& field) {
    return hbox({
        text(field.label) | border,
        text(INPUT_PADDING),
        field.com->Render() | flex,
    });
}

#endif
