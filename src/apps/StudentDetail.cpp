#include "apps/StudentDetail.hpp"
#include "viewmodel/my_view_model.hpp"
#include "apps/StudentList.hpp"
#include "apps/MoveStudent.hpp"
#include "apps/MainMenu.hpp"

unique_ptr<MoveStudent> student_detail_move_student = nullptr;

StudentDetail::StudentDetail(App* parent, unique_ptr<Student> student)
        : student(move(student)) {

    this->parent = parent;
    error_message = "";
    delete_btn = Button("Xoá sinh viên", [&] {
        try {
            auto student_db = ModelProducer::get_instance(ModelType::STUDENT);
            if (student_db) {
                if (student) {
                    student_db->remove(this->student.get());
                    
                    // Reload databse,
                    auto temp = dynamic_cast<StudentList*>(this->parent);
                    if (temp) {
                        temp->init_db();
                    }
                    

                    this->parent->run();
                } else {
                    error_message = "Không tìm thấy sinh viên!!";
                }
            } else {
                error_message = "Lỗi kết nối cơ sở dữ liệu!!";
            }
        }
        catch (const string& msg) {
            error_message = "Lỗi kết nối cơ sở dữ liệu!!";
        } catch (bad_alloc& ba) {
            error_message = "Lỗi cấp phát bộ nhớ!!";
        }
        catch (const exception& e) {
            error_message = "Lỗi không xác định!!";
        }
    }, ButtonOption::Animated(CANCEL_BTN_BG));
    
    registation_btn = Button("Đăng ký xe", [&] {
        // Do later.
    }, ButtonOption::Animated(CONFIRM_BTN_BG));
    
    payment_btn = Button("Thanh toán tiền phòng", [&] {
        // Do later.
    }, ButtonOption::Animated(CANCEL_BTN_BG));

    return_btn = Button("Quay lại", [&] {
        this->parent->run();
    }, ButtonOption::Animated(CONFIRM_BTN_BG));

    move_student_btn = Button("Đổi phòng", [&] {
        student_detail_move_student = make_unique<MoveStudent>(nullptr, this->student->get_id());
        main_menu::show();
        if (student_detail_move_student != nullptr) {
            debug("Running MoveStudent");
            student_detail_move_student->run();
        }
        debug("Doi phong success");
    }, ButtonOption::Animated(CANCEL_BTN_BG));
}

Element StudentDetail::create_element() {
    return vbox({
        get_title().get_doc() | color(TITLE_COLOR),
        separator(),
        hbox({
            vbox({
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
                text("Phòng: " + student->get_room()),
                text("Phương tiện:"), // Do later.
                text("Tiền phòng còn nợ:"), // Do later.
                text("Ghi chú: "), // Do later.
            }) | border | flex,
            vbox({
                delete_btn->Render() | flex,
                registation_btn->Render() | flex,
                payment_btn->Render() | flex,
                return_btn->Render() | flex,
                move_student_btn->Render() | flex,
            }) | border | flex,

        }),
        text(error_message) | center | color(ERROR_COLOR),
    });
}

bool StudentDetail::event(Event event) {
    return delete_btn->OnEvent(event)
        || registation_btn->OnEvent(event)
        || payment_btn->OnEvent(event)
        || return_btn->OnEvent(event)
        || move_student_btn->OnEvent(event);
}
