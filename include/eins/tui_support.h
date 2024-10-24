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

    void start(ScreenInteractive& screen);

    void stop();
    
    void add_component_tree(Component& component);
}

bool handle_console_size_changed();

#endif
