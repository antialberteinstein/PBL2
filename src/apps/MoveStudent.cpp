#include "apps/MoveStudent.hpp"
#include "apps/MainMenu.hpp"
#include "viewmodel/my_view_model.hpp"
#include "viewmodel/fee_calculator.hpp"
#include "models/RoomFeePayment.hpp"

MoveStudent::MoveStudent(App* parent, string id) {
    error_message = "";
    this->student_id = id;
    this->parent = parent;

    masoTf = {"Mã số", student_id, Input(&masoTf.value, "")};


    student_db = nullptr;

    confirm_btn = Button("Xác nhận", [&] {
        
        if (student != nullptr && room != nullptr) {
            
            try {
                auto rf_cal = FeeCalculator::get_instance(FeeType::ROOM_FEE);
                if (rf_cal) {
                    auto payment = rf_cal->get_payment(student.get());
                    if (payment != nullptr && payment->get_status() != PaymentStatus::PAID) {
                        error_message = "Sinh viên chưa thanh toán tiền phòng";
                        return;
                    }
                }
            } catch (const string& msg) {
                error_message = "Lỗi kết nối cơ sở dữ liệu!!";
                return;
            } catch (...) {
                error_message = "Lỗi không xác định!!";
                return;
            }


            student->set_room_id(room->get_id());
            try {
                if (this->student_db) {
                    this->student_db->modify(student->get_id(), student.get());
                }
                auto rf_cal = FeeCalculator::get_instance(FeeType::ROOM_FEE);
                if (rf_cal) {
                    rf_cal->get_payment(student.get());
                }
            } catch (const string& msg) {
                error_message = "Lỗi kết nối cơ sở dữ liệu!!";
            }
            if (this->parent != nullptr) {
                this->parent->run();
            } else {
                main_menu::show();
            }
        } else if (student == nullptr) {
            error_message = "Không tìm thấy sinh viên";
        } else if (room == nullptr) {
            error_message = "Không tìm thấy phòng";
        } else {
            error_message = "Vui lòng chọn sinh viên và phòng";
        }
        
    }, ButtonOption::Animated(CONFIRM_BTN_BG));


    return_btn = Button("Quay lại", [&] {
        if (this->parent != nullptr) {
            this->parent->run();
        } else {
            main_menu::show();
        }
    }, ButtonOption::Animated(CANCEL_BTN_BG));

    try {
        room_db = ModelProducer::get_instance(ModelType::ROOM);
    } catch (const string& msg) {
        try {
            room_db = ModelProducer::get_instance(ModelType::ROOM);
        } catch (const string& msg) {
            error_message = "Lỗi kết nối cơ sở dữ liệu!!";
        }
    }

    try {
        student_db = ModelProducer::get_instance(ModelType::STUDENT);
    } catch (const string& msg) {
        error_message = "Lỗi kết nối cơ sở dữ liệu!!";
    } catch (...) {
        error_message = "Lỗi kết nối cơ sở dữ liệu!!";
    }

    init_student_n_room();

    
    init_spinner();
    load_spinner_block();
    load_spinner_floor();
    load_spinner_room();

    old_student_id = "";

    event_listener = Container::Vertical({
        masoTf.com,
        chonKhuTf.com,
        chonTangTf.com,
        chonPhongTf.com,
        confirm_btn,
        return_btn,
    });
}

void MoveStudent::init_student_n_room() {
    if (student_db) {
        student = student_db->get_student(student_id);
    }

    room = nullptr;

    if (room_db != nullptr && student != nullptr) {
        room = room_db->get_room(student->get_room_id());
    }
}

Element MoveStudent::create_element() {
    auto student_detail = text("Không có thông tin sinh viên");
    if (student != nullptr) {
        student_detail = vbox({
            text("Mã số: " + student->get_id_string()),
            text("Họ và tên: " + student->get_name()),
            text("Ngày sinh: " + student->get_dob()),
            text("Số điện thoại: " + student->get_phone_number()),
            text("Quê quán: " + student->get_hometown()),
            text("Email: " + student->get_email()),
            text("Giới tính: " + student->get_gender()),
            text("Trường: " + student->get_university()),
            text("Ngành học: " + student->get_major()),
            text("Ngày vào ở: " + student->get_date_joined()),
            text("Phòng hiện tại: " + student->get_room()),
        }) | border | flex;
    }

    auto room_detail = text("Không có thông tin phòng");
    Elements _students;
    // -> Lay sinh vien o trong phong
    if (room != nullptr) {

        Vector<Student> students = 
            Student::get_students_living_in(room->get_id_string());
        Student::fit_room(room->get_id_string());
        for (int i = 0; i < students.size(); i++) {
            _students.push_back(text(
                "   " + to_string(i + 1) + "/. " + students[i].get_name()
                + " - " + students[i].get_gender()
                + " - " + students[i].get_hometown()
                ));
        }

        room_detail = vbox({
            text("Phòng: " + room->get_string_name()),
            text("Sức chứa: " + to_string(room->get_capacity())),
            text("Số sinh viên hiện tại: " + to_string(room->get_current_number())),
            text("Trạng thái: " + room->get_status()),
            (_students.size() == 0) ? text("Không có sinh viên") : vbox(_students),
        }) | border | flex;
    }

    return vbox({
        get_title().get_doc() | color(TITLE_COLOR),
        separator(),
        text(error_message) | center | color(ERROR_COLOR),
        hbox({
            vbox({
                get_doc(masoTf),
                student_detail | border,
                confirm_btn->Render(),
            }) | flex,
            vbox({
                get_doc(chonKhuTf) | flex,
                get_doc(chonTangTf) | flex,
                get_doc(chonPhongTf) | flex,
                room_detail | flex | border,
                return_btn->Render() | flex,
            }) | flex,
        }),
    });
}

bool MoveStudent::event(Event event) {
    if (old_student_id != masoTf.value) {
        old_student_id = masoTf.value;
        if (student_db) {
            auto _student = student_db->get_student(masoTf.value);
            if (_student != nullptr) {
                student = move(_student);
                if (room_db) {
                    room = room_db->get_room(student->get_room_id());
                }
            }
        }
    }

    update_spinner_block();
    update_spinner_floor();
    update_spinner_room();

    return event_listener->OnEvent(event);
}

void MoveStudent::update_spinner_room() {
    static string old_value = "";
    string new_value = Quarter::hash_to_id(
        chonKhuTf.values[chonKhuTf.selected],
        stoi(chonTangTf.values[chonTangTf.selected]),
        stoi(chonPhongTf.values[chonPhongTf.selected])
    );
    if (old_value != new_value) {
        old_value = new_value;
        if (room_db) {
            room = room_db->get_room(new_value);
        }
    }
}
