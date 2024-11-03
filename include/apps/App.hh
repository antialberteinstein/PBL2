#ifndef PBL_APP_HH
#define PBL_APP_HH

#include "eins/tui_support.h"
#include <iostream>

using namespace tui;
using namespace std;

class App {
    public:
        App() {}
        virtual ~App() {
        };
        virtual void run() {
            this->render_com = Renderer([&] {
                return this->create_element();
            }) | CatchEvent([&](Event event) {
                return this->event(event);
            });
            tui::add_component_tree(this->render_com);
        }
        virtual Element create_element() {
            return text("No render");
        }
        virtual bool event(Event event) {
            return false;
        }

    protected:
        Component render_com;
};

// This will carry the pointer of the app.
class AppAdapter {
    public:
        explicit AppAdapter(App* app=nullptr) : app(app) {}
        ~AppAdapter() {
            delete app;
            app = nullptr;
        }

        static App* connect(App* app) {
            static AppAdapter adapter;
            if (adapter.app) {
                delete adapter.app;
            }
            adapter.app = app;
            return adapter.app;
        }

    private:
        App* app;
};

Title& get_title();

#endif
