#ifndef PBL_TUI_OBJECT_HPP
#define PBL_TUI_OBJECT_HPP

#include "eins/tui_design/ftxui_includes.h"

namespace tui {
    class TuiObject {
        public:
            virtual Component& get_component() { return component; }
            virtual bool OnEvent(Event event) = 0;
        protected:
            Component component;
    };
}

#endif