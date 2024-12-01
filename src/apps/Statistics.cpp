#include "apps/Statistics.hpp"
#include "apps/MainMenu.hpp"
#include "viewmodel/my_view_model.hpp"
#include "apps/StudentDetail.hpp"
#include <sstream>



unique_ptr<StudentDetail> student_detail = nullptr;

Statistics::Statistics() {
    menu_items.push_back("1. Tìm kiếm nhanh sinh viên theo tên");
    menu_items.push_back("2. Quay lại menu chính");

    selected_index = 0;
    menu_option = MenuOption::Vertical();
    menu_option.selected = &selected_index;
    menu_option.focused_entry = &selected_index;

    menu_option.on_enter = [&] {
        switch (selected_index) {
            case 0: {
                search_result = nullptr;
                show_search_dialog = !show_search_dialog;
                break;
            }
            case 1: {
                main_menu::show();
                break;
            }
        }
    };

    menu = Menu(
        StringAdapter::From(&menu_items),
        &selected_index,
        menu_option);
    
    error_message = "";

    load_students();
    init_search_dialog();
}

Element Statistics::create_element() {
    Element temp = nullptr;
    if (show_search_dialog) {
        if (search_result != nullptr) {
            temp = vbox({
                text("Kết quả tìm kiếm:"),
                text("Mã số sinh viên: " + search_result->get_id_string()),
                text("Họ và tên: " + search_result->get_name()),
                text("Phòng: " + search_result->get_room()),
                search_result_btn->Render(),
            });
        } else {
            temp = hbox({
                search_dialog.com->Render() | flex,
                search_dialog_btn->Render(),
            });
        }
    }
    Elements bottom;
    bottom.push_back(menu->Render() | border | flex);
    if (temp != nullptr) {
        bottom.push_back(temp | border | flex);
    }

    return vbox({
        get_title().get_doc() | color(TITLE_COLOR),
        separator(),
        text(error_message) | center | color(ERROR_COLOR),
        separator(),
        hbox(bottom),
    });
}

bool Statistics::event(Event event) {
    return menu->OnEvent(event)
        || search_dialog_btn->OnEvent(event)
        || search_result_btn->OnEvent(event)
        || search_dialog.com->OnEvent(event);
}

void Statistics::init_search_dialog() {
    show_search_dialog = false;
    search_result = nullptr;
    search_dialog = {
        "Nhập tên sinh viên",
        "",
        Input(&search_dialog.value, "Tên sinh viên")
    };

    search_dialog_btn = Button("Tìm kiếm", [&] {
        search_students_by_name();
    }, ButtonOption::Animated(Color::Green3));

    search_result_btn = Button("Xem chi tiết sinh viên này", [&] {
        if (search_result != nullptr) {
            student_detail = make_unique<StudentDetail>(this, move(search_result));
            if (student_detail) {
                student_detail->run();
            }
        }
    }, ButtonOption::Animated(Color::Green3));
}

void Statistics::load_students() {
    try {
        auto student_db = ModelProducer::get_instance(ModelType::STUDENT);
        if (student_db) {
            student_db->unflag_students_sorted();
            Vector<string> keys = student_db->get_all_keys();
            for (int i = 0; i < keys.size(); ++i) {
                auto student = student_db->get_student(keys[i]);
                if (student) {
                    students.push_back(*student);
                }
            }
        }
    } catch (const string& msg) {
        error_message = msg;
    }
    regexes = regex_name(students);
}


void Statistics::search_students_by_name() {
    string search_name = search_dialog.value;
    if (search_name == "") {
        error_message = "Vui lòng nhập tên sinh viên cần tìm";
        return;
    }

    RegexingPattern search_pattern = regex_name(search_name);
    int index = binary_search(search_pattern, regexes);

    if (index == NOT_FOUND) {
        error_message = "Không tìm thấy sinh viên nào có tên " + search_name;
        return;
    }

    search_result = make_unique<Student>(students[index]);
    error_message = "Đã tìm thấy sinh viên " + search_name;
}