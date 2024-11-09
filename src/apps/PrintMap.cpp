#include "apps/PrintMap.hpp"
#include "apps/FileSelector.hpp"
#include "apps/MainMenu.hpp"
#include <filesystem>

#define MAP_FILE_PATH "./res/map/Map.jpg"

PrintMap::PrintMap() {
    path = "";
    is_file_selected = false;
    choosing_file_btn = Button("Chọn file", [&]() {
        try {
            path = get_save_file_path();
            is_file_selected = true;
        } catch (const char* e) {
            error_message = e;
        }
    });

    print_btn = Button("In", [&]() {
        if (is_file_selected) {
            std::filesystem::copy(MAP_FILE_PATH, path + "/");
            error_message = "Đã in xong.";
        }
    });

    back_btn = Button("Quay lại", [&]() {
        is_file_selected = false;
        main_menu::show();
    });

    event_listener = Container::Vertical({
        choosing_file_btn,
        back_btn,
        print_btn,
    });
}

Element PrintMap::create_element() {
    return vbox({
        get_title().get_doc() | color(TITLE_COLOR),
        separator(),
        hbox({
            choosing_file_btn->Render() | center | flex,
            back_btn->Render() | center | flex,
        }) | center | flex,
        text(path) | center | border,
        error_message.empty() ? text("") : text(error_message) | center | color(ERROR_COLOR),
        is_file_selected ? print_btn->Render() | center | flex : text(""),
    });
}

bool PrintMap::event(Event event) {
    return event_listener->OnEvent(event);
}