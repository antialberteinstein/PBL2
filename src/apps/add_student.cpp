#include "apps/add_student.hpp"
#include "apps/main_menu.hpp"

struct EditText {
    string label;
    string value;
    Component com;
};

Element get_doc_for_a_field(EditText& field) {
    return hbox({
        text(field.label),
        separator(),
        text(INPUT_PADDING),
        field.com->Render() | inverted,
    }) | border;
}

namespace add_student {

    Element foo = text("No event");

    void confirm_action() {
        foo = text("Confirmed");
    }

    void cancel_action() {
        main_menu::action();
    }

    Component container;
    EditText name, dob, gender, hometown, university, major, phone, email;
    Component confirm_btn, cancel_btn;

    Element create_element() {
        auto title_box = get_title().get_doc() | color(TITLE_COLOR);

        return vbox({
            title_box,
            separator(),
            vbox({
                hbox({
                    get_doc_for_a_field(name) | flex,
                    get_doc_for_a_field(dob) | flex,
                }) | flex,
                hbox({
                    get_doc_for_a_field(gender) | flex,
                    get_doc_for_a_field(hometown) | flex,
                }) | flex,
                hbox({
                    get_doc_for_a_field(university) | flex,
                    get_doc_for_a_field(major) | flex,
                }) | flex,
                hbox({
                    get_doc_for_a_field(phone) | flex,
                    get_doc_for_a_field(email) | flex,
                }) | flex,
            }),
            foo | center,
            hbox({
                confirm_btn->Render() | flex,
                text(BTN_PADDING),
                cancel_btn->Render() | flex,
            }) | center | flex,
        });
    }

    bool check_event(Event event) {
        return container->OnEvent(event);
    }

    void action() {
        name = {"Họ và tên", "", Input(&name.value, "Họ và tên")};
        dob = {"Ngày sinh", "", Input(&dob.value, "Ngày sinh")};
        gender = {"Giới tính", "", Input(&gender.value, "Giới tính")};
        hometown = {"Quê quán", "", Input(&hometown.value, "Quê quán")};
        university = {"Trường", "", Input(&university.value, "Trường")};
        major = {"Ngành học", "", Input(&major.value, "Ngành học")};
        phone = {"Số điện thoại", "", Input(&phone.value, "Số điện thoại")};
        email = {"Email", "", Input(&email.value, "Email")};

        confirm_btn = Button("Xác nhận", [&] {
            confirm_action();
        }, ButtonOption::Animated(CONFIRM_BTN_BG));

        cancel_btn = Button("Hủy", [&] {
            cancel_action();
        }, ButtonOption::Animated(CANCEL_BTN_BG));

        container = Container::Vertical({
            name.com,
            dob.com,
            gender.com,
            hometown.com,
            university.com,
            major.com,
            phone.com,
            email.com,
            confirm_btn,
            cancel_btn,
        });

        add_component_tree(container);
        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}
