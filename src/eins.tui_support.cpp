#include "eins/tui_support.h"
#include "functions.h"

bool handle_console_size_changed();

namespace console_size {
    int width = 0;
    int height = 0;
}

namespace tui {

    bool (*m_listener)(Event);
    Element (*m_render)();
    bool is_exit;

    void set_current_render_element(Element (*render)()) {
        m_render = render;
    }

    void set_event_listener(bool (*listener)(Event)) {
        m_listener = listener;
    }

    void start(ScreenInteractive& screen) {
        auto renderer = Renderer([&] {
            if (handle_console_size_changed()) {
                // Neu user thay doi kich thuoc console, xoa man hinh và render lai
                //      de tranh loi hien thi.
                clear_screen();
            }
            return m_render();
        });
        // Them su kien thoat khoi chuong trinh.
        renderer |= CatchEvent([&] (Event event) {
            if (event == Event::Character('q') || is_exit) {
                screen.ExitLoopClosure()();
                return true;
            }
            return m_listener(event);
        });
        screen.Loop(renderer);
    }

    void stop() {
        is_exit = true;
    }
    
    void init() {
        ALLOW_UTF8;
        m_render = [] { return text("Welcome to hell!"); };
        is_exit = false;
    }

    void cleanup() {
        // Do nothing
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
        for (int i = 0; i < size - 1; i++) {
            string str = padding + to_string(i + 1) + ". " + options[i].name;
            if (i == selected) {
                menu_elements.push_back(text(str) | inverted);
            } else {
                menu_elements.push_back(text(str));
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
