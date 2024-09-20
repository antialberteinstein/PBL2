#include "tui.h"
#include "functions.h"

bool handle_console_size_changed();

namespace console_size {
    int width = 0;
    int height = 0;
}

namespace tui {

   
    ifstream title_file("res/ascii_art/title.txt");
    
    void init() {
        ALLOW_UTF8;
    }

    void my_main(ScreenInteractive& screen) {

        /* Title */
        Elements title_elements;
        string line;
        while (getline(title_file, line)) {
            string tmp = line + "\n";
            title_elements.push_back(text(tmp) | center);
        }
        auto title_box = vbox(title_elements) | color(TITLE_COLOR);

        
        /* Menu */
        EMenu menu(10);

        menu.insert("Thêm sinh viên", do_nothing, "add_student");
        menu.insert("Đăng ký bao phòng", do_nothing, "room_reservation");
        menu.insert("Danh sách sinh viên", do_nothing, "student_list");
        menu.insert("Danh sách phòng", do_nothing, "room_list");
        menu.insert("Chuyển sinh viên sang phòng khác", do_nothing, "move_student");
        menu.insert("In tài liệu (*.csv)", do_nothing, "print_document");
        menu.insert("In bản đồ", do_nothing, "print_map");
        menu.insert("Thống kê", do_nothing, "statistics");
        menu.insert("Thoát", do_nothing, "exit");

        auto com = Renderer([&] {
            if (handle_console_size_changed()) {
                // Neu user thay doi kich thuoc console, xoa man hinh và render lai
                //      de tranh loi hien thi.
                clear_screen();
            }
            return
                vbox({
                    title_box,
                    separator(),
                    hbox({
                        menu.get_doc() | border | flex,
                        menu.get_desc() | border | flex,
                    }) | flex,
                });
        });

        com |= CatchEvent([&](Event event) {
            if (event == Event::Character('q')) {
                screen.ExitLoopClosure()();
                return true;
            } else if (event == Event::ArrowDown) {
                menu.move_down();
                return true;
            } else if (event == Event::ArrowUp) {
                menu.move_up();
                return true;
            } else if (event == Event::Return) {
                menu.select();
                return true;
            }
            return false;
        });

        screen.Loop(com);
    }

    void cleanup() {
        title_file.close();
    }

    // =================================================================
    //                           EMENU
    // =================================================================

    EMenu::EMenu(int capacity) {
        options = new MenuOption[capacity];
        size = 0;
        this->capacity = capacity;
        selected = 0;
        
    }

    EMenu::~EMenu() {
        delete[] options;
    }

    void EMenu::move_up() {
        selected = (selected - 1 + size) % size;
    }

    void EMenu::move_down() {
        selected = (selected + 1) % size;
    }

    void EMenu::select() {
        options[selected].action();
    }

    Element EMenu::get_doc() {
        Elements menu_elements;
        static const string padding = "   ";
        int index = 0;
        for (int i = 0; i < size; i++) {
            string str = padding + to_string(i + 1) + ". " + options[i].name;
            if (i == selected) {
                menu_elements.push_back(text(str) | inverted);
            } else {
                menu_elements.push_back(text(str) | color(get_color_ribbon()));
                index = (index + 1) % 3;
            }
        }
        return vbox(menu_elements);
    }

    void EMenu::insert(const string& name, func action,
            const string& desc_file_path, int index) {
        if (index < 0 || index > size) {
            index = size;
        }
        if (size == capacity) {
            return;
        }

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
        
        if (index == -1) {
            index = size;
        }
        for (int i = size; i > index; i--) {
            options[i] = options[i - 1];
        }

        options[index] = { name, action, desc };
        size++;
    }

    Element EMenu::get_desc() {
        return vbox(options[selected].desc);
    }

    Color get_color_ribbon() {
        static int index = 0;
        index = (index + 1) % 3;
        if (index == 0) {
            return MENU_COLOR_RIBBON_1;
        } else if (index == 1) {
            return MENU_COLOR_RIBBON_2;
        } else {
            return MENU_COLOR_RIBBON_3;
        }
        return MENU_COLOR_RIBBON_1;
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
