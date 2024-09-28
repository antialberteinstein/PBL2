#ifndef PBL_APP_HPP
#define PBL_APP_HPP

#include <iostream>
#include "eins/tui_support.h"

using namespace std;
using namespace tui;

void do_nothing();

Title& get_title();

/* Structure */
namespace app {
    void action();
}

#endif