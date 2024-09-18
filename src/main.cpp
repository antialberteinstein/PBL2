#include "main.h"
#include "tui.h"
 
int main(void) {
    tui::init();

    tui::my_main();
    tui::cleanup();
    return 0;
}