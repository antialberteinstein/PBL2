#include "apps/student_list.hpp"
#include <vector>
#include <ftxui/component/component_options.hpp>

#include "objects/Vector.hpp"
#include "objects/List.hpp"

#include "objects/Student.hpp"
#include "objects/Date.hpp"
#include "eins/sqlite3_support.h"
#include "objects/Adapter.hpp"
#include "apps/main_menu.hpp"

namespace student_list {
    Component container;
    Component menu_list;
    Vector<string> student_names;
    int selected = 0;
    MenuOption menu_option;
    List<Student> students;
    string error_message = "";
    string titles[] = {
        "ID",
        "Họ và tên",
        "Ngày sinh",
        "Giới tính",
        "Trường",
        "Ngành",
        "Quê quán",
        "Ngày nhận phòng",
    };
    int n = 8;
    Element the_title;
    Component return_btn;

    Element create_element() {
        return vbox({
            get_title().get_doc() | color(TITLE_COLOR),
            separator(),
            text(error_message) | center | color(ERROR_COLOR),
            return_btn->Render() | center,
            the_title | color(TITLE_COLOR) | bold | border | flex,
            menu_list->Render() | border | flex,
        });
    }

    void action() {
        return_btn = Button("Quay lại", [] {
            main_menu::action();
        });

        string foo = "";
        for (int i = 0; i < n; i++) {
            foo += titles[i] + "    ";
        }
        the_title = text(foo);

        try {
            if (!sql::check_connection()) {
                sql::reconnect();
            }
            auto conn = sql::get_connection();
            sql::QUERY query = sql::from_string("SELECT * FROM " + string(STUDENT_TABLE));
            sql::prepare(conn, query);
            students.clear();

            while (sql::next_row(sql::get_statement())) {
                Student s = Student::from_database(sql::get_statement());

                student_names.push_back(to_string(s.get_id()) + ".    " + s.get_name()
                    + "   " + s.get_dob().to_string() + "   "
                    + "   " + s.get_gender()
                    + "   " + s.get_university() + "   " + s.get_major()
                    + "   " + s.get_hometown() + "   "
                    + s.get_date_joined().to_string());
            }
            if (student_names.size() == 0) {
                error_message = "No students found";
            }
        } catch (sql::CreatingConnectionException& e) {
            error_message = e.get_message();
        } catch (sql::ExecutingQueryException& e) {
            error_message = e.get_message();
        }

        // for (int i = 0; i < students.size(); i++) {
        //     student_names.push_back(to_string(i + 1) + ".   " + students[i].get_name());
        // }

        menu_list = ftxui::Menu(Adapter::From(&student_names), &selected, menu_option);

        auto event_container = Container::Vertical({
            menu_list,
            return_btn,
        });

        container = Container::Vertical({
            Renderer([&] {
                return create_element();
            }),
        }) | CatchEvent([&](Event event) {
            return event_container->OnEvent(event);
        });

        add_component_tree(container);
    }
}