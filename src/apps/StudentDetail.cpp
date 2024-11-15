#include "apps/StudentDetail.hpp"

StudentDetail::StudentDetail(App* parent, unique_ptr<Student> student)
        : student(move(student)) {
    this->parent = parent;
    delete_btn = Button("Xoá sinh viên", [&] {
        // Do later.
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
            }) | border | flex,

        }),
    });
}

bool StudentDetail::event(Event event) {
    return delete_btn->OnEvent(event)
        || registation_btn->OnEvent(event)
        || payment_btn->OnEvent(event)
        || return_btn->OnEvent(event);
}
