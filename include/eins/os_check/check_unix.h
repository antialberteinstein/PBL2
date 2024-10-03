#ifndef CHECK_UNIX_H
#define CHECK_UNIX_H

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

#endif