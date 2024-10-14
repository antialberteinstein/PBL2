#ifndef PBL_EMENU_HPP
#define PBL_EMENU_HPP

#include "eins/tui_support.h"
#include "eins/tui_objects/TuiObject.hpp"
#include "objects/Vector.hpp"

namespace tui {
    class EMenu : public TuiObject {
        public:
            EMenu();
            Component& get_component() override;
            bool OnEvent(Event event) override;
            void select();
            void add(const string& name, func action, const string& desc_file_path);
            void clear_all();
        private:
            int selected;
            MenuOption options;
            Vector<string> labels;
            Vector<func> actions;
            Vector<Elements> descs;
            int noo;  // Number of options
    };
};

#endif