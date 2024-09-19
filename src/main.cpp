#include "main.h"
#include "tui.h"
 
int main(void) {
    tui::init();
    auto screen = ScreenInteractive::TerminalOutput();

    tui::my_main(screen);
    tui::cleanup();
    return 0;
}