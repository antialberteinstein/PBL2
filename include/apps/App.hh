#ifndef PBL_APP_HH
#define PBL_APP_HH

#include "eins/tui_support.h"
#include "eins/sqlite3_support.h"
#include <iostream>

using namespace tui;
using namespace std;

class App {
    public:
        App() {}
        virtual ~App() = default;
        virtual void run() {
            this->render_com = Renderer([&] {
                return this->create_element();
            }) | CatchEvent([&](Event event) {
                return this->event(event);
            });
            tui::add_component_tree(this->render_com);
        }
        virtual Element create_element() = 0;
        virtual bool event(Event event) = 0;

        static tui::Title get_title() {
            return tui::Title(TITLE_PATH, TITLE_TEXT);
        }
    protected:
        Component render_com;

};

#endif
