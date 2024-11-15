#ifndef PBL_PRINT_DOCUMENT_HPP
#define PBL_PRINT_DOCUMENT_HPP

#include "apps/App.hh"
#include "objects/Vector.hpp"
#include "objects/StringAdapter.hpp"

class ModelProducer;

class PrintDocument : public App {
    public:
        PrintDocument();
        virtual ~PrintDocument() = default;
        virtual Element create_element() override;
        virtual bool event(Event event) override;
    private:
        Component ddm_option_kinda_doc;
        Component choosing_file_btn;
        Component print_btn;
        Component back_btn;
        bool is_file_selected;
        string path;
        string error_message;
        Component event_listener;

        Vector<string> options;
        int selected;
};

#endif
