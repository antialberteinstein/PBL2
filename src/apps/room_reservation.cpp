#include "apps/room_reservation.hpp"

namespace room_reservation {
    Component container;
    Component dropdown_menu;
    int selected = 0;

    Element create_element() {
        auto title_box = get_title().get_doc() | color(TITLE_COLOR);

        return vbox({
            title_box,
            separator(),
            text("Room reservation")
        });
    }

    bool check_event(Event event) {
        return false;
    }

    void action() {
        /* string entries[] = {
            "Tran Nhat Nguyen",
        };

        dropdown_menu = Dropdown(&entries, &selected); */

        /* container = Container::Vertical({
            dropdown_menu,
        }); */

        /* add_component_tree(container); */
        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}