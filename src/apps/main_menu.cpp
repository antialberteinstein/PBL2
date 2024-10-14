#include "apps/main_menu.hpp"
#include "apps/add_student.hpp"
#include "apps/room_reservation.hpp"
#include "apps/student_list.hpp"
#include "apps/room_list.hpp"
#include "apps/move_student.hpp"
#include "apps/print_document.hpp"
#include "apps/print_map.hpp"
#include "apps/statistics.hpp"
#include "eins/tui_objects/EMenu.hpp"


namespace main_menu {

    EMenu menu;
    Component container;

    void action() {
        /* Menu */

        menu.clear_all();

        menu.add("Thêm sinh viên", add_student::action, "add_student");
        menu.add("Đăng ký bao phòng", room_reservation::action, "room_reservation");
        menu.add("Danh sách sinh viên", student_list::action, "student_list");
        menu.add("Danh sách phòng", room_list::action, "room_list");
        menu.add("Chuyển sinh viên sang phòng khác", move_student::action, "move_student");
        menu.add("In tài liệu (*.csv)", print_document::action, "print_document");
        menu.add("In bản đồ", print_map::action, "print_map");
        menu.add("Thống kê", statistics::action, "statistics");
        menu.add("Thoát", stop, "exit");

        container = Container::Vertical({
            Renderer([&] {
                return vbox({
                    get_title().get_doc() | color(TITLE_COLOR),
                    separator(),
                });
            }),
            menu.get_component(),
        });

        container |= CatchEvent([&](Event event) {
            return menu.OnEvent(event);
        });

        add_component_tree(container);
    }
}