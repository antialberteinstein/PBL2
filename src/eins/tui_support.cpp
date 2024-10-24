#include "eins/tui_support.h"

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
