#ifndef PBL_TUI_SUPPORT_H
#define PBL_TUI_SUPPORT_H

// Add os checking
#include "eins/os_check/check_unix.h"
#include "eins/os_check/check_windows.h"

// Add color definitions
#include "eins/tui_design/colors.h"

// Add string definitions
#include "eins/tui_design/strings.h"

// Add ftxui includes
#include "eins/tui_design/ftxui_includes.h"

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <memory>
#include "objects/List.hpp"

// Define sleep function
#define delay(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))
#define FRAME_CYCLE 1000/60

// Clear the console screen
#define clear_screen() system(CLEAR_CMD)

// Define the default list size.
#define LIST_MAX 10

using namespace std;

using func = void(*)();

typedef List<string> StringList;

namespace tui {

    // Initialize tui
    void init();

    // Clean up tui
    void cleanup();

    /* struct MenuOption {
        string name;
        func action;
        Elements desc;
    }; */

    class Title {
        private:
            Element doc;
            string base_text;
        public:
            Title(const string& path, const string& base_text="");
            Element get_doc();
    };

    
    /* class EMenu {
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
    }; */

    void start(ScreenInteractive& screen);

    void stop();
    
    void add_component_tree(Component& component);

    class TextField {
        string label;
        string value;
        Component com;
        static const string padding;
        public:
            TextField(const string& label="", const string& placeholder="");
            Element get_doc();
            Component& get_com();
            string get_value();
    };

    class Form {
        Component container;
        TextField* inputs;
        int size;
        int capacity;
        Component confirm_btn, cancel_btn;
        static const string btn_padding;
        public:
            Form(void (*confirm_action)(StringList output_values),
                void (*cancel_action)(), int capacity=LIST_MAX);
            ~Form();
            void add(TextField input);
            Element get_doc();
            Component& get_com();
            bool event_listener(Event event);
    };
}

bool handle_console_size_changed();

#endif
