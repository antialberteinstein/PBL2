#ifndef PBL_TUI_SUPPORT_H
#define PBL_TUI_SUPPORT_H

// Add os checking
#include "eins/os_check/check_unix.h"
#include "eins/os_check/check_windows.h"

// Add color definitions
#include "eins/tui_design/colors.h"

// Add string definitions
#include "eins/tui_design/strings.h"

// Add string list
#include "objects/StringList.hpp"

// Add ftxui includes
#include "eins/tui_design/ftxui_includes.h"

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

// Define sleep function
#define delay(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))
#define FRAME_CYCLE 1000/60

// Clear the console screen
#define clear_screen() system(CLEAR_CMD)

// Define the default list size.
#define LIST_MAX 10

using namespace std;

using func = void(*)();

namespace tui {

    // Initialize tui
    void init();

    // Clean up tui
    void cleanup();

    struct MenuOption {
        string name;
        func action;
        Elements desc;
    };

    class Title {
        private:
            Element doc;
            string base_text;
        public:
            Title(const string& path, const string& base_text="");
            Element get_doc();
    };

    
    class EMenu {
        private:
            MenuOption* options;
            int size;
            int capacity;
            int selected;

        public:
            EMenu(int capacity=LIST_MAX);
            ~EMenu();
            void insert(const string& name, func action,
                const string& desc_file_path, int index=-1);
            void move_up();
            void move_down();
            void select();
            Element get_doc();
            Element get_desc();
    };

    void set_current_render_element(Element (*create_element)());
    void set_event_listener(bool (*check_event)(Event event));

    void start(ScreenInteractive& screen);

    void stop();
    
    void add_component_tree(Component& component);
    
    class Form;

    class TextField {
        private:
            string label;
            string content;
            void add_text(const string& text);
            void backspace();
        public:
            TextField(const string& label="");
            TextField(const TextField& tf);
            Element get_doc();
            string get_text();
            void set_text(const string& str);
            friend class Form;
    };

    class Form {
        private:
            TextField* fields;
            int tf_size;
            int tf_capacity;
            int focused_index;

            /* Element btn_confirm;
            Element btn_cancel;
            int btn_confirm_index;
            int btn_cancel_index;

            bool focus_on_confirm();
            bool focus_on_cancel();
            void swap_btn_if_available(); */
        public:
            Form(int capacity=LIST_MAX);
            ~Form();
            void add_text_field(const string& label);
            void move_up();
            void move_down();
            Element get_doc();
            bool check_event(Event event);
            StringList retrieve_data() const;
    };
}

bool handle_console_size_changed();

#endif
