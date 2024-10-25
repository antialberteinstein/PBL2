#ifndef PBL_MAIN_MENU_HPP
#define PBL_MAIN_MENU_HPP

#include "apps/App.hh"
#include "eins/EMenu.hpp"

class MainMenu : public App {
    public:
        MainMenu();
        virtual Element create_element() override;
        virtual bool event(Event event) override;
    private:
        EMenu menu;
};

#endif