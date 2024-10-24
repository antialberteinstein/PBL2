#include "apps/main_menu.hpp"
#include "apps/add_student.hpp"


namespace main_menu {

    EMenu menu;
    Component container;

    void action() {
        /* Menu */

        menu.clear_all();

        menu.add("Thêm sinh viên", add_student::action, "add_student");
        menu.add("Đăng ký bao phòng", nullptr, "room_reservation");
        menu.add("Danh sách sinh viên", nullptr, "student_list");
        menu.add("Danh sách phòng", nullptr, "room_list");
        menu.add("Chuyển sinh viên sang phòng khác", nullptr, "move_student");
        menu.add("In tài liệu (*.csv)", nullptr, "print_document");
        menu.add("In bản đồ", nullptr, "print_map");
        menu.add("Thống kê", nullptr, "statistics");
        menu.add("Thoát", stop, "exit");

        container = Container::Vertical({
            Renderer([&] {
                return vbox({
                    App::get_title().get_doc() | color(TITLE_COLOR),
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
