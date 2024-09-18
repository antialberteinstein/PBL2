#ifndef PBL_TUI_H
#define PBL_TUI_H


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
#include <iostream>
#include <fstream>
#include "eins/Vector.hpp"
#include "eins/List.hpp"
#include "functions.h"
#include <thread>
#include <chrono>

// Define sleep function
#define delay(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))
#define FRAME_CYCLE 1000/60

using namespace std;
using namespace ftxui;

namespace tui {
    #define TITLE_HEIGHT 7


    // Initialize tui
    void init();

    // Main menu screen.
    void my_main();

    // Clean up tui
    void cleanup();

    struct MenuOption {
        string name;
        func action;
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
            void insert(MenuOption option, int index=-1);
            void move_up();
            void move_down();
            void select();
            Element get_doc();
    };
}

#endif // PBL_TUI_H