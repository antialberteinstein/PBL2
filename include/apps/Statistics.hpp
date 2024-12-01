#pragma once

#include "apps/App.hh"
#include "objects/Vector.hpp"
#include "models/Student.hpp"
#include "apps/components.hpp"
#include "models/Room.hpp"

struct RegexingPattern;


class Statistics : public App {
    public:
        Statistics();
        virtual ~Statistics() = default;
        virtual Element create_element() override;
        virtual bool event(Event event) override;
    private:

        Component menu;
        Vector<string> menu_items;
        int selected_index;
        MenuOption menu_option;

        string error_message;

        Vector<Student> students;
        Vector<RegexingPattern> regexes;

        void load_students();
        void load_rooms();


        // Tim kiem sinh vien theo ten.
        void search_students_by_name();
        bool show_search_dialog;
        EditText search_dialog;
        Component search_dialog_btn;
        unique_ptr<Student> search_result;
        Component search_result_btn;
        void init_search_dialog();





        // Analysis data
        int number_of_males;
        int number_of_females;
        int number_of_others;

        Vector<string> blocks;
        Vector<int> fulls;
        Vector<int> maintenances;
        int max_number_of_rooms;
};