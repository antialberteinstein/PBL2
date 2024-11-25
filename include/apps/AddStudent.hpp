#ifndef PBL_ADD_STUDENT_HPP
#define PBL_ADD_STUDENT_HPP

#include "apps/App.hh"
#include "objects/Vector.hpp"
#include "objects/Date.hpp"
#include "objects/StringAdapter.hpp"
#include "apps/components.hpp"

class ModelProducer;



class AddStudent : public App {
    public:
        AddStudent();
        virtual ~AddStudent() = default;
        virtual Element create_element() override;
        virtual bool event(Event event) override;
    private:
        Component confirm_btn,
                  cancel_btn,
                  event_listener;
        EditText name, dob, hometown,
                 university, major,
                 phone, email;
        ComboBox gender;
        string error_message;
        bool will_render;  // If false, render error message only
        ModelProducer* student_db;

};
#endif
