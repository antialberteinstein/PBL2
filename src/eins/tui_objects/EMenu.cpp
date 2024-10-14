#include "eins/tui_objects/EMenu.hpp"
#include "objects/Adapter.hpp"
#include "eins/tui_design/strings.h"
#include "eins/tui_design/colors.h"

tui::EMenu::EMenu() : selected(0), noo(0) {
    options.selected = &selected;
    options.focused_entry = &selected;
    this->component = Renderer([&] {
        return text("No render") | center;
    });
}

Component& tui::EMenu::get_component() {
    Component menu = Menu(Adapter::From(&labels), &selected, options) | border | flex;
    Component desc = Renderer([&] {
        return vbox(descs[selected]) | color(DESC_COLOR);
    }) | border | flex;

    component = Container::Horizontal({
        menu,
        desc,
    });

    return component;
}

bool tui::EMenu::OnEvent(Event event) {
    if (event.is_character()) {
        char c = event.character()[0];
        if (c >= '1' && c <= '9') {
            int index = c - '1';
            if (index < noo) {
                selected = index;
                return true;
            }
        }
        return false;
    } else if (event == Event::Return) {
        select();
        return true;
    } else if (selected == 0 && event == Event::ArrowUp) {
        selected = noo - 1;
        return true;
    } else if (selected == noo - 1 && event == Event::ArrowDown) {
        selected = 0;
        return true;
    } else {
        return component->OnEvent(event);
    }
}

void tui::EMenu::select() {
    actions.at(selected)();
}

void tui::EMenu::add(const string& name, func action, const string& desc_file_path) {
    labels.push_back(to_string(noo + 1) + "." + INPUT_PADDING  + name);
    actions.push_back(action);

    ifstream desc_file(desc_file_path);
    string line;
    Elements desc;

    if (!desc_file.good())
        desc_file = ifstream(DESCRIPTIONS_PATH + desc_file_path);
    if (!desc_file.good())
        desc_file = ifstream(DESCRIPTIONS_PATH + desc_file_path + ".txt");
    if (desc_file.good()) {
        while (getline(desc_file, line)) {
            desc.push_back(text(line));
        }
    }
    desc_file.close();

    descs.push_back(desc);
    noo++;
}

void tui::EMenu::clear_all() {
    labels.clear();
    actions.clear();
    descs.clear();
    noo = 0;
}
