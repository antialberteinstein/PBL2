#include "eins/tui_support.h"

bool handle_console_size_changed();

namespace console_size {
    int width = 0;
    int height = 0;
}

namespace tui {

    bool (*m_listener)(Event) = nullptr;
    Element (*m_render)() = nullptr;
    bool is_exit;
    Component tree;

    void set_current_render_element(Element (*create_element)()) {
        m_render = create_element;
    }

    void set_event_listener(bool (*check_event)(Event event)) {
        m_listener = check_event;
    }

   void add_component_tree(Component& con) {
        tree = con;
    } 

    void start(ScreenInteractive& screen) {
        auto renderer = Renderer(tree, [&] {
            delay(FRAME_CYCLE);
            if (handle_console_size_changed()) {
                // Neu user thay doi kich thuoc console, xoa man hinh và render lai
                //      de tranh loi hien thi.
                clear_screen();
            }
            if (m_render != nullptr)
                return m_render();
            else
                return text("Cannot render anything!!");
        });
        // Them su kien thoat khoi chuong trinh.
        renderer |= CatchEvent([&] (Event event) {
            if (event == Event::Character('q') || is_exit) {
                screen.ExitLoopClosure()();
                return true;
            }
            if (m_listener != nullptr)
                return m_listener(event);
            else
                return false;
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
        for (int i = 0; i < size; i++) {
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
        if (size == capacity - 1) {
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

    // =========================================================
    //                    TEXT_FIELD
    // =========================================================
    
    const string TextField::padding = INPUT_PADDING;

    TextField::TextField(const string& label, const string& placeholder) {
        this->label = label;
        value = "";
        com = Input(&value, placeholder);
    }

    Element TextField::get_doc() {
        return hbox({
            text(label),
            separator(),
            text(padding),
            com->Render() | inverted,
        }) | border | flex;
    }

    Component& TextField::get_com() {
        return com;
    }

    string TextField::get_value() {
        return value;
    }

    // =========================================================
    //                    FORM
    // =========================================================

    const string Form::btn_padding = BTN_PADDING;

    Form::Form(void (*confirm_action)(StringList output_values),
            void (*cancel_action)(), int capacity) {
        this->inputs = new TextField[capacity];
        this->capacity = capacity;
        this->size = 0;
        
        this->confirm_btn = Button(CONFIRM_BTN_TEXT, [&] {
            StringList output_values(size);
            for (int i = 0; i < size; i++) {
                output_values.add(inputs[i].get_value());
            }
            confirm_action(output_values);
        }, ButtonOption::Animated(CONFIRM_BTN_BG));

        this->cancel_btn = Button(CANCEL_BTN_TEXT, [&] {
            cancel_action();
        }, ButtonOption::Animated(CANCEL_BTN_BG));

        this->container = Container::Vertical({});
        /* this->container->Add(confirm_btn);
        this->container->Add(cancel_btn); */
    }

    Form::~Form() {
        delete[] inputs;
    }

    void Form::add(TextField field) {
        if (size >= capacity) {
            return;
        }
        inputs[size++] = field;
        this->container->Add(field.get_com());
    }

    Element Form::get_doc() {
        Elements form_elements;
        for (int i = 0; i < size; i += 2) {
            if (i + 1 < size) {
                form_elements.push_back(hbox({
                    inputs[i].get_doc() | flex,
                    text(btn_padding),
                    inputs[i + 1].get_doc() | flex,
                }));
            } else {
                form_elements.push_back(inputs[i].get_doc());
            }
        }
        form_elements.push_back(hbox({
            confirm_btn->Render() | flex,
            text(btn_padding),
            cancel_btn->Render() | flex,
        }) | center | flex);
        return vbox(form_elements);
    }

    Component& Form::get_com() {
        return container;
    }

    bool Form::event_listener(Event event) {
        return container->OnEvent(event);
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
