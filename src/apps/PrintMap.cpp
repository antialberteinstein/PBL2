#include "apps/PrintMap.hpp"
#include "apps/FileSelector.hpp"
#include "apps/MainMenu.hpp"
#include <filesystem>
#include "objects/Date.hpp"

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
            Date date = Date::today();
            string base_output_path = path + "/"
                + "Map_" + DateConverter::to_string(date);
            string output_path = base_output_path + ".jpg";
            int counter = 1;
            while (filesystem::exists(output_path)) {
                output_path = base_output_path + "(" + to_string(counter) + ").jpg";
                counter++;
            }
            std::filesystem::copy(MAP_FILE_PATH, output_path);
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