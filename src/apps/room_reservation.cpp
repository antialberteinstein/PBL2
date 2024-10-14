#include "apps/room_reservation.hpp"
#include <vector>

namespace room_reservation {
    Component container;
    Component dropdown_menu;
    int selected = 0;

    Element create_element() {
        auto title_box = get_title().get_doc() | color(TITLE_COLOR);

        return vbox({
            title_box,
            separator(),
            text("Room reservation"),
            dropdown_menu->Render(),
        });
        // return container->Render();
    }

    bool check_event(Event event) {
        return container->OnEvent(event);
    }

    void action() {
        std::vector<string> entries = {
            "Tran Nhat Nguyen",
        };

        dropdown_menu = Dropdown(entries, &selected);

        container = Container::Vertical({
            dropdown_menu,
        });

        add_component_tree(container);
    }
}