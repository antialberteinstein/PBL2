#include "apps/room_list.hpp"
#include "objects/Vector.hpp"
#include "objects/Adapter.hpp"
#include "eins/sqlite3_support.h"
#include "objects/Room.hpp"

namespace room_list {

    Component container;
    Component menu_list;
    Vector<string> room_names;
    int selected = 0;
    MenuOption menu_option;
    string error_message;

    Element create_element() {
        return vbox({
            get_title().get_doc() | color(TITLE_COLOR),
            separator(),
            menu_list->Render() | border | flex,
            text(error_message) | center | color(ERROR_COLOR),
        });
    }

    bool check_event(Event event) {
        return menu_list->OnEvent(event);
    }

    void action() {
        room_names.clear();
        try {
            if (!sql::check_connection()) {
                sql::reconnect();
            }
            auto conn = sql::get_connection();
            sql::QUERY query = sql::from_string("SELECT * FROM " + string(ROOM_TABLE));
            sql::prepare(conn, query);

            while (sql::next_row(sql::get_statement())) {
                Room room = Room::from_database(sql::get_statement());
                room_names.push_back(room.get_block() + to_string(room.get_floor() * 100 + room.get_number()));
            }
        } catch (sql::CreatingConnectionException& e) {
            error_message = e.get_message();
        } catch (sql::ExecutingQueryException& e) {
            error_message = e.get_message();
        }

        menu_list = Menu(Adapter::From(&room_names), &selected, menu_option);

        container = Container::Vertical({
            Renderer([&] {
                return create_element();
            }),
        }) | CatchEvent([&](Event event) {
            return check_event(event);
        });

        add_component_tree(container);
    }
}