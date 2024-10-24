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

    void action() {
        std::vector<string> entries = {
            "Room 1",
            "Room 2",
            "Room 3",
            "Room 4",
            "Room 5",
        };

        dropdown_menu = Dropdown(entries, &selected);

        container = Container::Vertical({
            Renderer([&] {
                return create_element();
            }),
        }) | CatchEvent([&](Event event) {
            return dropdown_menu->OnEvent(event);
        });

        add_component_tree(container);
    }
}