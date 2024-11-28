#pragma once

#include "apps/App.hh"
#include "models/Student.hpp"
#include "apps/components.hpp"

class StudentDetail : public App {
    public:
        StudentDetail(App* parent, unique_ptr<Student> student);
        virtual ~StudentDetail() = default;
        virtual Element create_element() override;
        virtual bool event(Event event) override;

        void reload_student_from_db();
    private:
        unique_ptr<Student> student;
        Component delete_btn;  // Xoa sinh vien.
        Component registation_btn;  // Dang ky xe.
        Component payment_btn;  // Thanh toan.
        Component return_btn;  // Quay lai.
        Component move_student_btn;  // Doi phong.
        Component update_student_btn;  // Cap nhat thong tin sinh vien.

        string error_message;

        App* parent;

};
