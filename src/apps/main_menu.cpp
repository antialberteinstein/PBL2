#include "apps/main_menu.hpp"
#include "apps/add_student.hpp"
#include "apps/room_reservation.hpp"
#include "apps/student_list.hpp"
#include "apps/room_list.hpp"
#include "apps/move_student.hpp"
#include "apps/print_document.hpp"
#include "apps/print_map.hpp"
#include "apps/statistics.hpp"


namespace main_menu {

    EMenu menu(10);

    Element create_element() {

        /* Title */
        auto title_box = get_title().get_doc() | color(TITLE_COLOR);


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

        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}