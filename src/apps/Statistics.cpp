#include "apps/Statistics.hpp"
#include "apps/MainMenu.hpp"
#include "viewmodel/my_view_model.hpp"
#include "apps/StudentDetail.hpp"
#include <sstream>

Element male_female_other(int male, int female, int other) {
    int total = male + female + other;
    int max = male;
    if (female > max)  max = female;
    if (other > max)  max = other;

    return vbox({
        hbox({
            text("Nam:   "),
            gauge((float)male/max),
            text("  " + to_string(male) + " sinh viên"),
        }) | color(Color::Blue3),
        hbox({
            text("Nữ:    "),
            gauge((float)female/max),
            text("  " + to_string(female) + " sinh viên"),
        }) | color(Color::Orange4),
        hbox({
            text("Khác:  "),
            gauge((float)other/max),
            text("  " + to_string(other) + " sinh viên"),
        }) | color(Color::GrayLight),
        hbox({
            text("Tổng số: "),
            text("  " + to_string(total) + " sinh viên"),
        }) | color(Color::YellowLight),
    }) | border | flex;
}

Element room_maintenance_or_not(
    int max_number_of_rooms,
    Vector<string>& blocks,
    Vector<int>& maintenances
) {
    Elements foo;
    foo.push_back(text("Số lượng phòng bảo trì:"));
    for (int i = 0; i < blocks.size(); ++i) {
        string baz = to_string(maintenances[i]) + "/" + to_string(max_number_of_rooms);
        if (maintenances[i] >= 100) {
            baz = to_string(maintenances[i]) + "/" + to_string(max_number_of_rooms);
        } else if (maintenances[i] > 10) {
            baz = " " + to_string(maintenances[i]) + "/" + to_string(max_number_of_rooms);
        } else {
            baz = "  " + to_string(maintenances[i]) + "/" + to_string(max_number_of_rooms);
        }
        auto bar = hbox({
            text("Khu   " + blocks[i] + ": "),
            gauge((float)maintenances[i]/max_number_of_rooms) | color(Color::Red3) | bgcolor(Color::Green4),
            text(baz),
        });
        foo.push_back(bar);
    }

    int total = 0;
    for (int i = 0; i < maintenances.size(); ++i) {
        total += maintenances[i];
    }
    foo.push_back(hbox({
        text("Tổng số: "),
        text(" " + to_string(total) + "/" + to_string(max_number_of_rooms)),
    }));

    return vbox(foo) | border | flex;
}

Element room_full_or_not(
    int max_number_of_rooms,
    Vector<string>& blocks,
    Vector<int>& fulls
) {
    Elements foo;
    foo.push_back(text("Số lựợng phòng có đủ sinh viên:"));
    for (int i = 0; i < blocks.size(); ++i) {
        string baz;
        if (fulls[i] >= 100) {
            baz = to_string(fulls[i]) + "/" + to_string(max_number_of_rooms);
        } else if (fulls[i] > 10) {
            baz = " " + to_string(fulls[i]) + "/" + to_string(max_number_of_rooms);
        } else {
            baz = "  " + to_string(fulls[i]) + "/" + to_string(max_number_of_rooms);
        }
        auto bar = hbox({
            text("Khu   " + blocks[i] + ": "),
            gauge((float)fulls[i]/max_number_of_rooms) | color(Color::DeepSkyBlue1) | bgcolor(Color::GrayDark),
            text(baz),
        });
        foo.push_back(bar);
    }

    int total = 0;
    for (int i = 0; i < fulls.size(); ++i) {
        total += fulls[i];
    }

    foo.push_back(hbox({
        text("Tổng số: "),
        text(" " + to_string(total) + "/" + to_string(max_number_of_rooms)),
    }));

    return vbox(foo) | border | flex;
}

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
    load_rooms();
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
        hbox({
            male_female_other(number_of_males, number_of_females, number_of_others),
            room_maintenance_or_not(max_number_of_rooms, blocks, maintenances),
            room_full_or_not(max_number_of_rooms, blocks, fulls),
        }) | flex,
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
    number_of_females = 0;
    number_of_males = 0;
    number_of_others = 0;
    try {
        auto student_db = ModelProducer::get_instance(ModelType::STUDENT);
        if (student_db) {
            student_db->unflag_students_sorted();
            Vector<string> keys = student_db->get_all_keys();
            for (int i = 0; i < keys.size(); ++i) {
                auto student = student_db->get_student(keys[i]);
                if (student) {
                    students.push_back(*student);
                    if (student->get_gender() == "Nam") {
                        number_of_males++;
                    } else if (student->get_gender() == "Nữ") {
                        number_of_females++;
                    } else if (student->get_gender() == "Khác") {
                        number_of_others++;
                    }
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

void Statistics::load_rooms() {
    blocks.clear();
    blocks.push_back("A");
    blocks.push_back("B");
    blocks.push_back("C");
    blocks.push_back("D");
    blocks.push_back("E");
    blocks.push_back("F");

    fulls.clear();
    fulls.push_back(0);
    fulls.push_back(0);
    fulls.push_back(0);
    fulls.push_back(0);
    fulls.push_back(0);

    maintenances.clear();
    maintenances.push_back(0);
    maintenances.push_back(0);
    maintenances.push_back(0);
    maintenances.push_back(0);
    maintenances.push_back(0);

    max_number_of_rooms = 24 * 5;
    fulls.clear();
    maintenances.clear();

    try {
        auto room_db = ModelProducer::get_instance(ModelType::ROOM);
        if (room_db) {
            Vector<string> keys = room_db->get_all_keys();

            for (int i = 0; i < keys.size(); ++i) {
                auto room = room_db->get_room(keys[i]);
                if (room) {
                    for (int i = 0; i < blocks.size(); ++i) {
                        if (room->get_block() == blocks[i]) {
                            if (room->get_status() == RoomStatus::FULL) {
                                fulls[i]++;
                            }
                            if (room->get_status() == RoomStatus::MAINTENANCE) {
                                maintenances[i]++;
                            }
                        }
                    }
                }
            }
        }
    } catch (const string& msg) {
        error_message = msg;
    }
}
