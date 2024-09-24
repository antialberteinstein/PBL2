#include "main.h"
#include "tui.h"
#include "db_connector.h"
#include "objects/Rooms.hpp"

int main(void) {
    tui::init();
    auto screen = ScreenInteractive::TerminalOutput();

    tui::my_main(screen);
    tui::cleanup();

    //rooms::RoomList room_list = rooms::RoomList::from_db();
    //room_list.show();

    return 0;
}