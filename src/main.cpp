#include "eins/tui_support.h"
#include "apps/MainMenu.hpp"
#include "apps/AppFactory.hpp"
#include "models/Student.hpp"

#include "objects/Date.hpp"

#include "viewmodel/my_view_model.hpp"

bool pbl();

int main(void) {

    pbl();

    

    return 0;
}



bool pbl() {
     {
    tui::init();
    auto screen = ScreenInteractive::TerminalOutput();

    main_menu::show();

    tui::start(screen);

    tui::cleanup();

    return true;
}
}
