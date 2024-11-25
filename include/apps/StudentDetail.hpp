#pragma once

#include "apps/App.hh"
#include "models/Student.hpp"

class StudentDetail : public App {
    public:
        StudentDetail(App* parent, unique_ptr<Student> student);
        virtual ~StudentDetail() = default;
        virtual Element create_element() override;
        virtual bool event(Event event) override;
    private:
        unique_ptr<Student> student;
        Component delete_btn;  // Xoa sinh vien.
        Component registation_btn;  // Dang ky xe.
        Component payment_btn;  // Thanh toan.
        Component return_btn;  // Quay lai.
        Component move_student_btn;  // Doi phong.

        string error_message;

        App* parent;

};
