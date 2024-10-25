#include "apps/MainMenu.hpp"

MainMenu::MainMenu() {
    menu = EMenu();

    menu.add(AppType::ADD_STUDENT);
    menu.add(AppType::ROOM_RESERVATION);
    menu.add(AppType::STUDENT_LIST);
    menu.add(AppType::ROOM_LIST);
    menu.add(AppType::MOVE_STUDENT);
    menu.add(AppType::PRINT_DOCUMENT);
    menu.add(AppType::PRINT_MAP);
    menu.add(AppType::STATISTICS);
    menu.add(AppType::EXIT);
}

Element MainMenu::create_element() {
    return vbox({
        get_title().get_doc() | color(TITLE_COLOR),
        separator(),
        menu.get_component()->Render(),
    });
}

bool MainMenu::event(Event event) {
    return menu.OnEvent(event);
}