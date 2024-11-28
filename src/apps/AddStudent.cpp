#include "apps/AddStudent.hpp"
#include "apps/App.hh"
#include "apps/MainMenu.hpp"
#include "viewmodel/my_view_model.hpp"
#include "apps/MoveStudent.hpp"
#include "apps/StudentDetail.hpp"

unique_ptr<MoveStudent> add_student_move_student = nullptr;

AddStudent::AddStudent(App* parent, string student_id) {
    will_render = true;

    // Lam 2 lan moi khong loi :)))))
    try {
        student_db = ModelProducer::get_instance(ModelType::STUDENT);
    } catch (const runtime_error& e) {
        try {
            student_db = ModelProducer::get_instance(ModelType::STUDENT);
        } catch (const runtime_error& e) {
            error_message = "Lỗi kết nối cơ sở dữ liệu!!";
            will_render = false;
        }
    }
    name = {"Họ và tên", "", Input(&name.value, "Họ và tên")};
    dob = {"Ngày sinh", "", Input(&dob.value, "Ngày sinh")};
    hometown = {"Quê quán", "", Input(&hometown.value, "Quê quán")};
    university = {"Trường", "", Input(&university.value, "Trường")};
    major = {"Ngành học", "", Input(&major.value, "Ngành học")};
    phone = {"Số điện thoại", "", Input(&phone.value, "Số điện thoại")};
    email = {"Email", "", Input(&email.value, "Email")};
    gender.label = "Giới tính";
    gender.values.push_back("Nam");
    gender.values.push_back("Nữ");
    gender.values.push_back("Khác");
    gender.selected = 1;
    gender.com = Dropdown(StringAdapter::From(&gender.values), &gender.selected);

    this->parent = parent;
    if (student_id == "") {
        this->student = nullptr;
    } else {
        if (this->student_db) {
            this->student = this->student_db->get_student(student_id);
        }
        if (this->student != nullptr) {
            name.value = this->student->get_name();
            dob.value = this->student->get_dob();
            hometown.value = this->student->get_hometown();
            university.value = this->student->get_university();
            major.value = this->student->get_major();
            phone.value = this->student->get_phone_number();
            email.value = this->student->get_email();

            if (this->student->get_gender() == "Nam") {
                gender.selected = 0;
            } else if (this->student->get_gender() == "Nữ") {
                gender.selected = 1;
            } else {
                gender.selected = 2;
            }
        }
    }

    confirm_btn = Button("Xác nhận", [&] {
        if (name.value.empty() || dob.value.empty() || hometown.value.empty() ||
            university.value.empty() || major.value.empty() || phone.value.empty() ||
            email.value.empty()) {
            error_message = "Vui lòng điền đầy đủ thông tin!!";
            return;
        }

        if (student_db == nullptr) {
            error_message = "Lỗi kết nối cơ sở dữ liệu!!";
            return;
        }

        if (this->student != nullptr) {
            this->student->set_name(name.value);
            this->student->set_dob(dob.value);
            this->student->set_gender(gender.values[gender.selected]);
            this->student->set_hometown(hometown.value);
            this->student->set_university(university.value);
            this->student->set_major(major.value);
            this->student->set_phone_number(phone.value);
            this->student->set_email(email.value);

            student->turn_off_creating_flag();

            try {
                student_db->modify(student->get_id(), student.get());
            } catch (const string& msg) {
                error_message = msg;
                return;
            }
            if (this->parent != nullptr) {
                auto temp = dynamic_cast<StudentDetail*>(this->parent);
                if (temp) {
                    temp->reload_student_from_db();
                }

                this->parent->run();
            } else {
                main_menu::show();
            }
            return;
        }
        else {
            auto student = make_unique<Student>();
            student->set_name(name.value);
            student->set_dob(dob.value);
            student->set_gender(gender.values[gender.selected]);
            student->set_hometown(hometown.value);
            student->set_university(university.value);
            student->set_major(major.value);
            student->set_phone_number(phone.value);
            student->set_email(email.value);
            student->set_room_id(NO_ROOM_ID);
            student->set_date_joined(Date::today().to_string());

            student->turn_on_creating_flag();

            try {
                student_db->add(student.get());
                main_menu::show();
                add_student_move_student = make_unique<MoveStudent>(nullptr, student->get_id());
                if (add_student_move_student != nullptr) {
                    add_student_move_student->run();
                }
            } catch (const string& msg) {
                error_message = msg;
                return;
            }
        }

        
    }, ButtonOption::Animated(CONFIRM_BTN_BG));

    cancel_btn = Button("Hủy", [&] {
        // Return to main menu
        main_menu::show();
    }, ButtonOption::Animated(CANCEL_BTN_BG));

    event_listener = Container::Vertical({
        name.com, dob.com,
        gender.com, hometown.com,
        university.com, major.com,
        phone.com, email.com,
        confirm_btn, cancel_btn,
    });
}

bool AddStudent::event(Event event) {
    return event_listener->OnEvent(event);
}

Element AddStudent::create_element() {
    auto title_box = get_title().get_doc() | color(TITLE_COLOR);

    if (!will_render) {
        return vbox({
            title_box,
            separator(),
            text(error_message) | center | color(ERROR_COLOR),
            cancel_btn->Render() | center | flex,
        });
    }

    return vbox({
        title_box,
        separator(),
        vbox({
            hbox({
                get_doc(name) | flex,
                get_doc(dob) | flex,
            }) | flex,
            hbox({
                get_doc(gender) | flex,
                get_doc(hometown) | flex,
            }) | flex,
            hbox({
                get_doc(university) | flex,
                get_doc(major) | flex,
            }) | flex,
            hbox({
                get_doc(phone) | flex,
                get_doc(email) | flex,
            }) | flex,
        }),
        hbox({
            confirm_btn->Render() | flex,
            text(BTN_PADDING),
            cancel_btn->Render() | flex,
        }) | center | flex,
        text(error_message) | center | color(ERROR_COLOR),
    });
}
