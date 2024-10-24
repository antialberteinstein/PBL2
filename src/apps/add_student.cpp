#include "apps/add_student.hpp"
#include "apps/main_menu.hpp"
#include "objects/Vector.hpp"
#include "objects/StringAdapter.hpp"
#include "objects/Date.hpp"
#include "objects/Student.hpp"
#include "eins/sqlite3_support.h"

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

    Component container;
    EditText name, dob, hometown, university, major, phone, email;
    ComboBox gender;
    Component confirm_btn, cancel_btn;
    Component event_listener;
    string error_message;

    void confirm_action() {
        if (name.value.empty() || dob.value.empty() || hometown.value.empty() ||
            university.value.empty() || major.value.empty() || phone.value.empty() ||
            email.value.empty()) {
            error_message = "Vui lòng điền đầy đủ thông tin!!";
            // return;
        }

        Date today(0, 0, 0);

        try {
            today = Date::today();
        } catch (GettingLocalTimeException& e) {
            error_message = e.get_message();
            return;
        }

        Student new_student = Student(
            STUDENT_NULL_VALUE, name.value,
            DateConverter::from_grenadian_string(dob.value),
            phone.value, email.value,
            university.value, major.value,
            hometown.value, today,
            false, 0, STUDENT_NULL_VALUE, gender.values[gender.selected]
        );
        try {
            if (!sql::check_connection()) {
                sql::reconnect();
            }
            auto conn = sql::get_connection();
            bool check = Student::database_insert(conn, new_student);
            if (check) {
                // Return to main menu
                main_menu::action();

            } else {
                throw sql::ExecutingQueryException("Failed to insert student into database");
            }
        } catch (sql::ExecutingQueryException& e) {
            error_message = e.get_message() + sqlite3_errmsg(sql::get_connection());
        } catch (sql::CreatingConnectionException& e) {
            error_message = e.get_message() + sqlite3_errmsg(sql::get_connection());
        }
    }

    void cancel_action() {
        main_menu::action();
    }

    Element create_element() {
        auto title_box = App::get_title().get_doc() | color(TITLE_COLOR);

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
        gender.com = Dropdown(StringAdapter::From(&gender.values), &gender.selected);

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
