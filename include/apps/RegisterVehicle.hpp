#pragma once

#include "apps/App.hh"
#include "apps/components.hpp"
#include "models/Student.hpp"
#include "models/Vehicle.hpp"
#include "objects/Vector.hpp"

class RegisterVehicle : public App {
    public:
        RegisterVehicle(App* parent, string student_id);
        virtual ~RegisterVehicle() = default;
        virtual Element create_element() override;
        virtual bool event(Event event) override;
    private:

        EditText masoTf;
        ComboBox loaiXeTf;
        Component confirm_btn;
        Component return_btn;

        Component vehicles_menu;
        Vector<string> vm_list;
        int vm_selected;
        MenuOption vm_option;
        Component delete_btn;

        string error_message;

        // Fur text change event.
        string old_student_id;

        App* parent;
        string student_id;
};