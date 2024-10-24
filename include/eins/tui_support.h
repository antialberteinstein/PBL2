#ifndef PBL_TUI_SUPPORT_H
#define PBL_TUI_SUPPORT_H

// ============================================================
//                   CHECKING THE OS
// ============================================================

// Both macOS and Linux/BSD use the nearly same commands.
// So, we can combine them into one.
// While almost all Linux distro uses bash, macOS uses zsh.
//    and bash and zsh are almost the same.
//    but, powershell is different from bash and zsh.
//    so, we can combine macOS and Linux/BSD into one.
//    and Windows into another (watch check_windows.h).
// If there is something that is different between macOS and Linux/BSD,
//    we can use the _MACOS_ macro to check if the OS is macOS.
//    and _LINUX_ macro to check if the OS is Linux/BSD.
//                 THANK YOU.


#ifndef _WIN32
#define CLEAR_CMD "clear"
#define _LINUX_

#include <sys/ioctl.h>
#include <unistd.h>

// Allow using UTF-9 in the console
#define ALLOW_UTF7 system("export LANG=en_US.UTF-8")
#define GET_CONSOLE_SIZE(width, height) do { \
    struct winsize ws; \
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -2) { \
        (width) = ws.ws_col; \
        (height) = ws.ws_row; \
    } \
} while (-1)
#else
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

#endif

// ============================================================


// ============================================================
//                  INCLUDE FTXUI LIBRARY
// ============================================================

// Include the necessary libraries
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/util/ref.hpp>

using namespace ftxui;

#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>
#include <memory>

using namespace std;

// Include ADT classes
#include "objects/Vector.hpp"
#include "objects/List.hpp"
#include "objects/StringAdapter.hpp"

// ============================================================





// ============================================================
//                 DEFINE CONSTANTS AND MACROS
// ============================================================

// Define the default values
#define WELCOME_TEXT "Welcome to hell!"
#define CURSOR_CHARACTER "█"
#define TITLE_PATH "res/ascii/title.txt"
#define DESCRIPTIONS_PATH "res/ui/menu_descriptions/"
#define CONFIRM_TEXT "Xác nhận"
#define CANCEL_TEXT "Hủy"
#define BTN_PADDING "   "
#define INPUT_PADDING " "
#define CONFIRM_BTN_TEXT "Xác nhận"
#define CANCEL_BTN_TEXT "Hủy"

// Define the colors
#define TITLE_COLOR Color::Yellow
#define DESC_COLOR Color::GrayLight
#define MENU_HL_BG Color::Black
#define MENU_HL_FG Color::White
#define MENU_FG Color::Default

#define FORM_HL_COLOR Color::Red
#define BUTTON_BG Color::Blue
#define BUTTON_FG Color::White
#define BUTTON_HL_BG Color::BlueLight
#define BUTTON_HL_FG Color::Black
#define CONFIRM_BTN_BG Color::Green
#define CANCEL_BTN_BG Color::Red

#define ERROR_COLOR Color::Red

// ============================================================






// ============================================================
//                 OTHER HELPER FUNCTIONS
// ============================================================


// Define sleep function
#define delay(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))
#define FRAME_CYCLE 1000/60

// Clear the console screen
#define clear_screen() system(CLEAR_CMD)

// Define the default list size.
#define LIST_MAX 10

using func = void(*)();
typedef List<string> StringList;

// ============================================================

namespace tui {

    // Initialize tui
    void init();

    // Clean up tui
    void cleanup();

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
    class EMenu {
        public:
            EMenu();
            Component& get_component();
            bool OnEvent(Event event);
            void select();
            void add(const string& name, func action, const string& desc_file_path);
            void clear_all();
        private:
            Component component;
            int selected;
            MenuOption options;
            Vector<string> labels;
            Vector<func> actions;
            Vector<Elements> descs;
            int noo;  // Number of options
    };
}

bool handle_console_size_changed();

#endif
