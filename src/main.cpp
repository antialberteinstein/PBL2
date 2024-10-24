#include "main.h"
#include "eins/tui_support.h"
#include "apps/main_menu.hpp"
#include "eins/sqlite3_support.h"

#include "objects/Date.hpp"


int main(void) {
    sql::connect(DB_PATH);
    tui::init();
    auto screen = ScreenInteractive::TerminalOutput();

    main_menu::action();

    tui::start(screen);

    tui::cleanup();
    sql::disconnect();

    system("pause");

    return 0;
}