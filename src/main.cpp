#include "eins/tui_support.h"
#include "eins/sqlite3_support.h"
#include "apps/MainMenu.hpp"
#include "apps/AppFactory.hpp"

#include "objects/Date.hpp"


int main(void) {
    sql::connect(DB_PATH);
    tui::init();
    auto screen = ScreenInteractive::TerminalOutput();

    main_menu::show();

    tui::start(screen);

    tui::cleanup();
    sql::disconnect();

    return 0;
}
