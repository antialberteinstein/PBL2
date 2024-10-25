#include "eins/tui_support.h"
#include "eins/sqlite3_support.h"
#include "apps/MainMenu.hpp"
#include "apps/AppFactory.hpp"

#include "objects/Date.hpp"


int main(void) {
    sql::connect(DB_PATH);
    tui::init();
    auto screen = ScreenInteractive::TerminalOutput();

    AppAdapter::connect(new MainMenu())->run();

    tui::start(screen);

    tui::cleanup();
    sql::disconnect();

    return 0;
}
