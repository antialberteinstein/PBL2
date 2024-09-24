#include "functions.h"

using namespace tui;

Title title("res/ascii_art/title.txt", "Quan ly KTX");

void do_nothing() {
    return;
}

namespace main_menu {

    EMenu menu(10);

    Element create_element() {

        /* Title */
        auto title_box = title.get_doc() | color(TITLE_COLOR);

        
        /* Menu */

        menu.insert("Thêm sinh viên", add_student::action, "add_student");
        menu.insert("Đăng ký bao phòng", room_reservation::action, "room_reservation");
        menu.insert("Danh sách sinh viên", student_list::action, "student_list");
        menu.insert("Danh sách phòng", room_list::action, "room_list");
        menu.insert("Chuyển sinh viên sang phòng khác", move_student::action, "move_student");
        menu.insert("In tài liệu (*.csv)", print_document::action, "print_document");
        menu.insert("In bản đồ", print_map::action, "print_map");
        menu.insert("Thống kê", statistics::action, "statistics");
        menu.insert("Thoát", stop, "exit");

        return vbox({
                title_box,
                separator(),
                hbox({
                    menu.get_doc() | border | flex,
                    menu.get_desc() | border | flex,
                }) | flex,
        });
    }

    bool check_event(Event event) {
        if (event == Event::ArrowDown) {
            menu.move_down();
            return true;
        } else if (event == Event::ArrowUp) {
            menu.move_up();
            return true;
        } else if (event == Event::Return) {
            menu.select();
            return true;
        }
        return false;
    }

    void action() {
        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}

namespace add_student {
    Element create_element() {
        return text("Add student");
    }

    bool check_event(Event event) {
        return false;
    }

    void action() {
        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}

namespace room_reservation {
    Element create_element() {
        return text("Room reservation");
    }

    bool check_event(Event event) {
        return false;
    }

    void action() {
        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}

namespace student_list {
    Element create_element() {
        return text("Student list");
    }

    bool check_event(Event event) {
        return false;
    }

    void action() {
        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}

namespace room_list {
    Element create_element() {
        return text("Room list");
    }

    bool check_event(Event event) {
        return false;
    }

    void action() {
        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}

namespace move_student {
    Element create_element() {
        return text("Move student");
    }

    bool check_event(Event event) {
        return false;
    }

    void action() {
        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}

namespace print_document {
    Element create_element() {
        return text("Print document");
    }

    bool check_event(Event event) {
        return false;
    }

    void action() {
        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}

namespace print_map {
    Element create_element() {
        return text("Print map");
    }

    bool check_event(Event event) {
        return false;
    }

    void action() {
        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}

namespace statistics {
    Element create_element() {
        return text("Statistics");
    }

    bool check_event(Event event) {
        return false;
    }

    void action() {
        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}
