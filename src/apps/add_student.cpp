#include "apps/add_student.hpp"
#include "apps/main_menu.hpp"
#include "objects/Vector.hpp"
#include "objects/Adapter.hpp"

struct EditText {
    string label;
    string value;
    Component com;
};

struct ComboBox {
    string label;
    Vector<string> values;
    int selected;
    Component com;
};

Element get_doc(EditText& field) {
    return hbox({
        text(field.label),
        separator(),
        text(INPUT_PADDING),
        field.com->Render() | inverted,
    }) | border;
}

Element get_doc(ComboBox& field) {
    return hbox({
        text(field.label) | border,
        text(INPUT_PADDING),
        field.com->Render() | flex,
    });
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
    EditText name, dob, hometown, university, major, phone, email;
    ComboBox gender;
    Component confirm_btn, cancel_btn;
    Component event_listener;

    Element create_element() {
        auto title_box = get_title().get_doc() | color(TITLE_COLOR);

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
            foo | center,
            hbox({
                confirm_btn->Render() | flex,
                text(BTN_PADDING),
                cancel_btn->Render() | flex,
            }) | center | flex,
        });
    }

    void action() {
        

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
        gender.com = Dropdown(Adapter::From(&gender.values), &gender.selected);

        confirm_btn = Button("Xác nhận", [&] {
            confirm_action();
        }, ButtonOption::Animated(CONFIRM_BTN_BG));

        cancel_btn = Button("Hủy", [&] {
            cancel_action();
        }, ButtonOption::Animated(CANCEL_BTN_BG));

        container = Renderer([&] {
            return create_element();
        });

        event_listener = Container::Vertical({
            name.com, dob.com,
            gender.com, hometown.com,
            university.com, major.com,
            phone.com, email.com,
            confirm_btn, cancel_btn,
        });

        container |= CatchEvent([&] (Event event) {
            return event_listener->OnEvent(event);
        });

        add_component_tree(container);
    }
}
