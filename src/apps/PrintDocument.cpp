#include "apps/PrintDocument.hpp"
#include "apps/FileSelector.hpp"
#include "apps/MainMenu.hpp"
#include "viewmodel/my_view_model.hpp"
#include "models/Room.hpp"
#include "models/Student.hpp"


#define PM_OPTION_1 "In danh sách sinh viên (*.csv)"
#define PM_OPTION_2 "In danh sách phòng (*.csv)"

void print_student_list(Vector<Student> students);

void print_room_list(Vector<Room> rooms);

PrintDocument::PrintDocument() {
    options.push_back(PM_OPTION_1);
    options.push_back(PM_OPTION_2);

    ddm_option_kinda_doc = Dropdown(
        StringAdapter::From(&options),
        &selected    
    );

    // Start to load data. 


    path = "";
    is_file_selected = false;
    choosing_file_btn = Button("Chọn file", [&]() {
        try {
            path = get_save_file_path();
            is_file_selected = true;
        } catch (const char* e) {
            error_message = e;
        }
    });

    print_btn = Button("In", [&]() {
        if (is_file_selected) {
            if (options[selected] == PM_OPTION_1) {
                Vector<Student> students;

                print_student_list(students);
            }
            else if (options[selected] == PM_OPTION_2) {
                Vector<Room> rooms;

                print_room_list(rooms);
            }

            error_message = "Đã in xong.";
        }
    });

    back_btn = Button("Quay lại", [&]() {
        is_file_selected = false;
        main_menu::show();
    });

    event_listener = Container::Vertical({
        ddm_option_kinda_doc,   
        choosing_file_btn,
        back_btn,
        print_btn,
    });
}

Element PrintDocument::create_element() {
    return vbox({
        get_title().get_doc() | color(TITLE_COLOR),
        separator(),
        ddm_option_kinda_doc->Render() | center | flex,
        hbox({
            choosing_file_btn->Render() | center | flex,
            back_btn->Render() | center | flex,
        }) | center | flex,
        text(path) | center | border,
        error_message.empty() ? text("") : text(error_message) | center | color(ERROR_COLOR),
        is_file_selected ? print_btn->Render() | center | flex : text(""),
    });
}

bool PrintDocument::event(Event event) {
    return event_listener->OnEvent(event);
}


void print_student_list(Vector<Student> students) {
    //
}

void print_room_list(Vector<Room> rooms) {
    //
}
