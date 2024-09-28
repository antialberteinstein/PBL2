#include "apps/app.hpp"

Title title("res/ascii_art/title.txt", "Quan ly KTX");

void do_nothing() {
    // Do nothing
}

Title& get_title() {
    return title;
}

namespace app {
    void action() {
        // Do nothing
    }
}