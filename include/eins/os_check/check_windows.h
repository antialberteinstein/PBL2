#ifndef CHECK_WINDOWS_H
#define CHECK_WINDOWS_H

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

#endif

#endif