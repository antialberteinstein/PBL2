#ifndef PBL_TUI_SUPPORT_H
#define PBL_TUI_SUPPORT_H

// ======================================================
//             Check for Windows.
// ======================================================

#ifdef _WIN32
// Windows
#define CLEAR_CMD "cls"

// Allow using UTF-8 in the console
#include <windows.h>
#define ALLOW_UTF8 {\
    SetConsoleOutputCP(CP_UTF8);\
    SetConsoleCP(CP_UTF8);\
}

#define _WINDOWS_

#define GET_CONSOLE_SIZE(width, height) do { \
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); \
    if (hConsole != INVALID_HANDLE_VALUE) { \
        CONSOLE_SCREEN_BUFFER_INFO csbi; \
        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) { \
            (width) = csbi.dwSize.X; \
            (height) = csbi.dwSize.Y; \
        } \
    } \
} while (0)








// ======================================================
//             Check for macOS.
// ======================================================

#elif __APPLE__
// Mac OS
#define CLEAR_CMD "clear"
#define _MACOS_

#include <sys/ioctl.h>
#include <unistd.h>

// Allow using UTF-8 in the console
#define ALLOW_UTF8 system("export LANG=en_US.UTF-8")
#define GET_CONSOLE_SIZE(width, height) do { \
    struct winsize ws; \
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1) { \
        (width) = ws.ws_col; \
        (height) = ws.ws_row; \
    } \
} while (0)


// ======================================================
//             Check for Linux or BSD.
// ======================================================

#else
// Linux or BSD
#define CLEAR_CMD "clear"
#define _LINUX_

#include <sys/ioctl.h>
#include <unistd.h>

// Allow using UTF-8 in the console
#define ALLOW_UTF8 system("export LANG=en_US.UTF-8")
#define GET_CONSOLE_SIZE(width, height) do { \
    struct winsize ws; \
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1) { \
        (width) = ws.ws_col; \
        (height) = ws.ws_row; \
    } \
} while (0)

#endif

// Clear the console screen
#define clear_screen() system(CLEAR_CMD)

// Include the necessary libraries
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/util/ref.hpp>
#include <iostream>
#include <fstream>
#include "functions.h"
#include <thread>
#include <chrono>

// Define sleep function
#define delay(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))
#define FRAME_CYCLE 1000/60

#define DESCRIPTIONS_PATH "res/ui/menu_descriptions/"
#define TITLE_COLOR Color::Yellow

// Define the default list size.
#define LIST_MAX 10
#define CURSOR_CHARACTER "█"

using namespace std;
using namespace ftxui;

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

    void set_current_render_element(Element (*render)());

    void set_event_listener(bool (*listener)(Event));

    void start(ScreenInteractive& screen);

    void stop();
    
    void add_component_tree(Component component);
    
    class Form;

    class TextField {
        private:
            string label;
            string text;
            void add_text(const string& text);
            void backspace();
        public:
            TextField(const string& label);
            Element get_doc();
            string get_text();
            void set_text(const string& text);
            friend class Form;
    };

    class Form {
        private:
            TextField* fields;
            int tf_size;
            int tf_capacity;
            int focused_index;
        public:
            Form(int capacity=2);
            ~Form();
            void add_text_field(const string& label);
            void move_up();
            void move_down();
            Element get_doc();
            void event_listener(Event event);
            string* retrieve_data();
    }
}

bool handle_console_size_changed();

#endif
