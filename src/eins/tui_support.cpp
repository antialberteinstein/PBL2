#include "eins/tui_support.h"

bool handle_console_size_changed();

namespace console_size {
    int width = 0;
    int height = 0;
}

namespace tui {

    bool (*m_listener)(Event);
    Element (*m_render)();
    bool is_exit;
    Component tree;

    void set_current_render_element(Element (*create_element)()) {
        m_render = create_element;
    }

    void set_event_listener(bool (*check_event)(Event)) {
        m_listener = check_event;
    }

   void add_component_tree(Component con) {
        tree = con;
    } 

    void start(ScreenInteractive& screen) {
        auto renderer = Renderer(tree, [&] {
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
        tree = Container::Vertical({});
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
        return vbox(options[selected].desc) | color(DESC_COLOR);
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
    

    // =============================================================
    //                 TEXT_VIEW
    // =============================================================

    TextField::TextField(const string& label) {
        this->label = label;
        this->content = "";
    }

    TextField::TextField(const TextField& tf) {
        this->label = tf.label;
        this->content = tf.content;
    }

    Element TextField::get_doc() {
        return hbox({
            text(label),
            separator(),
            text(content)
        }) | flex | border;
    }

    string TextField::get_text() {
        return content;
    }

    void TextField::set_text(const string& str) {
        content = str;
    }

    void TextField::add_text(const string& str) {
        content += str;
    }

    void TextField::backspace() {
        if (content != "") {
            content.pop_back();
        }
    }

    // ==========================================================
    //                 FORM
    // ==========================================================

    Form::Form(int capacity) {
        this->fields = new TextField[capacity];
        this->tf_capacity = capacity;
        this->tf_size = 0;
        this->focused_index = 0;
    }

    Form::~Form() {
        delete []fields;
    }

    void Form::add_text_field(const string& label) {
        if (tf_size >= tf_capacity)
            return;
        this->fields[tf_size++] = TextField(label);
    }

    void Form::move_up() {
        focused_index = (focused_index - 1 + tf_size) % tf_size;
    }

    void Form::move_down() {
        focused_index = (focused_index + 1) % tf_size;
    }

    Element Form::get_doc() {
        Elements form_elements;
        for (int i = 0; i < tf_size; i++) {
            if (i == focused_index) {
                form_elements.push_back(fields[i].get_doc() | color(FORM_HL_COLOR));
            } else {
                form_elements.push_back(fields[i].get_doc());
            }
        }
        return vbox(form_elements);
    }

    bool Form::check_event(Event event) {
        if (event == Event::Character('\t') || event == Event::Return) {
            move_down();
            return true;
        } else if (event == Event::ArrowUp) {
            move_up();
            return true;
        } else if (event == Event::ArrowDown) {
            move_down();
            return true;
        } else if (event.is_character()) {
            fields[focused_index].add_text(event.character());
            return true;
        } else if (event == Event::Backspace) {
            fields[focused_index].backspace();
            return true;
        }
        return false;
    }

    StringList Form::retrieve_data() const {
        StringList sl(tf_size);
        for (int i = 0; i < tf_size; i++) {
            sl[i] = fields[i].get_text();
        }
        return sl;
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
