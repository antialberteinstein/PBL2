#include "apps/add_student.hpp"

namespace add_student {

    void confirm_action(StringList output_values) {
        // Do nothing
    }

    void cancel_action() {
        // Do nothing
    }

    // Form form(20);
    /* string name_placeholder = "Họ và tên";
    string var_name = "";
    Component input_name;

    string dob_placeholder = "Ngày sinh";
    string var_dob = "";
    Component input_dob;

    Component confirm_btn, cancel_btn;

    Component container; */
    Form* form_ptr;

    Element create_element() {
        auto title_box = get_title().get_doc() | color(TITLE_COLOR);

        /*
        return vbox({
            title_box,
            separator(),
            form.get_doc(),
        }); */

        /* s */

        Element content = (form_ptr != nullptr) ?
            form_ptr->get_doc() : text("Lỗi không xác định");

        return vbox({
            title_box,
            separator(),
            content,
        });
    }

    bool check_event(Event event) {
        // return form.check_event(event);/*  */
        // return container->OnEvent(event);
        if (form_ptr == nullptr) {
            return false;
        }
        return form_ptr->event_listener(event);
    }

    void action() {
        /* input_name = Input(&var_name, name_placeholder);
        input_dob = Input(&var_dob, dob_placeholder);
        confirm_btn = Button("Xác nhận", [&] {
            var_name = "Lê Nhã Phương";
        }, ButtonOption::Animated(Color::Green));
        cancel_btn = Button("Hủy", [&] {
            var_name = "Trần Nhật Nguyên";
        }, ButtonOption::Animated(Color::Red));

        container = Container::Vertical({
            input_name,
            input_dob,
            confirm_btn,
            cancel_btn,
        }); */

        Form form(confirm_action, cancel_action, 20);

        form.add(TextField("Họ và tên", "Họ và tên"));
        form.add(TextField("Ngày sinh", "Ngày sinh"));
        form.add(TextField("Giới tính", "Giới tính"));
        form.add(TextField("Quê quán", "Quê quán"));
        form.add(TextField("Trường", "Trường"));/* 
        form.add(TextField("Ngành học", "Ngành học"));
        form.add(TextField("Số điện thoại", "Số điện thoại"));
        form.add(TextField("Email", "Email")); */

        form_ptr = &form;

        add_component_tree(form.get_com());
        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}
