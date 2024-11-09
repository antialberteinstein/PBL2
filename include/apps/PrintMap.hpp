#ifndef PBL_PRINT_MAP_HPP
#define PBL_PRINT_MAP_HPP

#include "apps/App.hh"

class PrintMap : public App {
    public:
        PrintMap();
        virtual ~PrintMap() = default;
        virtual Element create_element() override;
        virtual bool event(Event event) override;
    private:
        Component choosing_file_btn;
        Component print_btn;
        Component back_btn;
        bool is_file_selected;
        string path;
        string error_message;
        Component event_listener;
};

#endif // PBL_PRINT_MAP_HPP