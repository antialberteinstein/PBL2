#include "main.h"
#include "eins/tui_support.h"
#include "objects/Rooms.hpp"
#include "apps/main_menu.hpp"

int main(void) {
    tui::init();
    auto screen = ScreenInteractive::TerminalOutput();

    main_menu::action();

    tui::start(screen);

    tui::cleanup();

    return 0;
}