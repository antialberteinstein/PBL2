#include "eins/tui_support.h"
#include "eins/EMenu.hpp"

bool handle_console_size_changed();

namespace console_size {
    int width = 0;
    int height = 0;
}

namespace tui {

    bool is_exit;
    Component tree;

   void add_component_tree(Component& con) {
        tree = con;
    }

    void start(ScreenInteractive& screen) {
        auto renderer = Renderer(tree, [&] {
            if (is_exit) {
                screen.ExitLoopClosure()();
                return text("Exiting...");
            }
            delay(FRAME_CYCLE);
            if (handle_console_size_changed()) {
                // Neu user thay doi kich thuoc console, xoa man hinh và render lai
                //      de tranh loi hien thi.
                clear_screen();
            }
            return tree->Render();
        });
        // Them su kien thoat khoi chuong trinh.
        renderer |= CatchEvent([&] (Event event) {
            if (event == Event::CtrlC || event == Event::Escape) {
                stop();
                return true;
            }
            return tree->OnEvent(event);
        });

        screen.Loop(renderer);

    }

    void stop() {
        is_exit = true;
    }

    void init() {
        ALLOW_UTF8;
        is_exit = false;
        tree = Container::Vertical({});
    }

    void cleanup() {
        // Do nothing
    }

    // =========================================================
    //                    TITLE
    // =========================================================
    Title::Title(const string& path, const string& base_text) {
        this->base_text = base_text;
        ifstream file(path);
        Elements title_elements;

        if (file.good()) {
            string line;
            while(getline(file, line)) {
                title_elements.push_back(text(line) | center);
            }
            doc = vbox(title_elements);
        } else {
            doc = text(base_text) | center;
        }

        file.close();
    }

    Element Title::get_doc() {
        return doc;
    }
}

bool handle_console_size_changed() {
    int width, height;
    GET_CONSOLE_SIZE(width, height);
    if (width != console_size::width || height != console_size::height) {
        console_size::width = width;
        console_size::height = height;
        return true;
    }
    return false;
}

tui::EMenu::EMenu() : selected(0), noo(0) {
    options.selected = &selected;
    options.focused_entry = &selected;
    this->component = Renderer([&] {
        return text("No render") | center;
    });
}

Component& tui::EMenu::get_component() {
    Component menu = Menu(StringAdapter::From(&labels), &selected, options) | border | flex;
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
    auto action_app = AppFactory::produce(this->types.at(selected));
    if (action_app)
        action_app->run();
}

void tui::EMenu::add(AppType type) {
    string name = LabelFactory::produce(type);
    string desc_file_path = DescriptionFactory::produce(type);

    labels.push_back(to_string(noo + 1) + "." + INPUT_PADDING  + name);
    types.push_back(type);

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
    types.clear();
    descs.clear();
    noo = 0;
}
