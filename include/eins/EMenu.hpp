#ifndef PBL_EMENU_HPP
#define PBL_EMENU_HPP

#include "eins/tui_support.h"
#include "apps/AppFactory.hpp"

namespace tui {
    class EMenu {
            public:
                EMenu();
                Component& get_component();
                bool OnEvent(Event event);
                void select();
                void add(AppType type);
                void clear_all();
            private:
                Component component;
                int selected;
                MenuOption options;
                Vector<string> labels;
                Vector<AppType> types;
                Vector<Elements> descs;
                int noo;  // Number of options
    };
}

#endif
