#include "apps/StudentDetail.hpp"
#include "viewmodel/my_view_model.hpp"
#include "apps/StudentList.hpp"
#include "apps/MoveStudent.hpp"
#include "apps/MainMenu.hpp"
#include "viewmodel/fee_calculator.hpp"
#include "models/RoomFeePayment.hpp"
#include "apps/AddStudent.hpp"

unique_ptr<MoveStudent> student_detail_move_student = nullptr;
unique_ptr<AddStudent> student_detail_update_student = nullptr;

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
                    
                    // Reload database,
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
    }, ButtonOption::Animated(Color::Red3));

    update_student_btn = Button("Cập nhật thông tin", [&] {
        student_detail_update_student = make_unique<AddStudent>(this, this->student->get_id());
        main_menu::show();
        if (student_detail_update_student != nullptr) {
            student_detail_update_student->run();
        }
    }, ButtonOption::Animated(Color::Green3));
    
    registation_btn = Button("Đăng ký xe", [&] {
        // Do later.
    }, ButtonOption::Animated(Color::Green1));
    
    payment_btn = Button("Thanh toán tiền phòng", [&] {
        try {
            auto rf_cal = FeeCalculator::get_instance(FeeType::ROOM_FEE);
            if (rf_cal != nullptr && this->student != nullptr) {
                rf_cal->pay_by(this->student.get());
                error_message = "Đã thanh toán!! Tính năng in hoá đơn đang được cập nhật hoặc chưa hỗ trợ, quý khách vui lòng thông cảm...";
            } else {
                error_message = "Chưa thanh toán được!!";
            }
        } catch (const string& msg) {
            error_message = msg;
        } catch (...) {
            error_message = "Lỗi không xác định!!";
        }
    }, ButtonOption::Animated(Color::Green3));

    return_btn = Button("Quay lại", [&] {
        auto temp = dynamic_cast<StudentList*>(this->parent);
        if (temp) {
            temp->init_db();
        }

        this->parent->run();
    }, ButtonOption::Animated(Color::Orange1));

    move_student_btn = Button("Đổi phòng", [&] {
        student_detail_move_student = make_unique<MoveStudent>(this, this->student->get_id());
        main_menu::show();
        if (student_detail_move_student != nullptr) {
            debug("Running MoveStudent");
            student_detail_move_student->run();
        }
        debug("Doi phong success");
    }, ButtonOption::Animated(Color::DeepSkyBlue2));
}

Element StudentDetail::create_element() {
    // Element for student's room fee.
    string room_fee = "Không có";
    try {
        auto rf_cal = FeeCalculator::get_instance(FeeType::ROOM_FEE);
        debug("Created FeeCalculator");
        if (rf_cal) {
            auto payment = rf_cal->get_payment(student.get());
            debug("Created Payment");
            if (payment != nullptr && payment->get_status() != PaymentStatus::PAID) {
                if (payment->get_status() == PaymentStatus::OVERDUE) {
                    room_fee = to_string(payment->get_amount()) + " VND (Quá hạn)";
                } else {
                    room_fee = to_string(payment->get_amount()) + " VND (Chưa thanh toán)";
                }
                debug("Finished counting room fee");
            }
        }
    } catch (const string& msg) {
        error_message = msg;
    } catch (...) {
        error_message = "Lỗi không xác định!!";
    }

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
                text("Tiền phòng còn nợ: " + room_fee),
                text("Ghi chú: Không có ghi chú"), // Do later.
            }) | border | flex,
            vbox({
                delete_btn->Render() | flex,
                update_student_btn->Render() | flex,
                registation_btn->Render() | flex,
                payment_btn->Render() | flex,
                return_btn->Render() | flex,
                move_student_btn->Render() | flex,
            }) | flex,

        }),
        text(error_message) | center | color(ERROR_COLOR),
    });
}

bool StudentDetail::event(Event event) {
    return delete_btn->OnEvent(event)
        || registation_btn->OnEvent(event)
        || payment_btn->OnEvent(event)
        || return_btn->OnEvent(event)
        || move_student_btn->OnEvent(event)
        || update_student_btn->OnEvent(event);
}

void StudentDetail::reload_student_from_db() {
    try {
        auto student_db = ModelProducer::get_instance(ModelType::STUDENT);
        if (student_db) {
            this->student = student_db->get_student(this->student->get_id());
        }
    } catch (const string& msg) {
        error_message = msg;
    } catch (...) {
        error_message = "Lỗi không xác định!!";
    }
}