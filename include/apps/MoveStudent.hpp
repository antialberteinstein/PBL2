#pragma once

#include "apps/App.hh"
#include "apps/components.hpp"
#include "models/Student.hpp"
#include "models/Room.hpp"
#include "apps/MySpinner.hpp"

class ModelProducer;

class MoveStudent : public App {
    public:
        MoveStudent(App* parent=nullptr, string id="");
        virtual ~MoveStudent() = default;
        virtual Element create_element() override;
        virtual bool event(Event event) override;
    private:

        EditText masoTf;

        ComboBox chonKhuTf;
        ComboBox chonTangTf;
        ComboBox chonPhongTf;

        shared_ptr<SpinnerTreeNode> room_tree;
        
        Component confirm_btn;
        Component return_btn;
        unique_ptr<Student> student;
        unique_ptr<Room> room;

        ModelProducer* student_db;
        ModelProducer* room_db;

        string error_message;

        // Fur text change event.
        string old_student_id;

        Component event_listener;

        App* parent;
        string student_id;

        void init_spinner() {
            room_tree = MySpinner::spin();
            chonKhuTf = {
                "Chọn khu",
                Vector<string>(), 0,
                Dropdown(StringAdapter::From(&chonKhuTf.values), &chonKhuTf.selected)
            };

            chonTangTf = {
                "Chọn tầng",
                Vector<string>(), 0,
                Dropdown(StringAdapter::From(&chonTangTf.values), &chonTangTf.selected)
            };

            chonPhongTf = {
                "Chọn phòng",
                Vector<string>(), 0,
                Dropdown(StringAdapter::From(&chonPhongTf.values), &chonPhongTf.selected)
            };
        }

        void load_spinner_block() {
            chonKhuTf.values.clear();
            for (int i = 0; i < room_tree->children.size(); i++) {
                chonKhuTf.values.push_back(room_tree->children[i]->data);
            }
            chonKhuTf.selected = 0;
        }

        void load_spinner_floor() {
            chonTangTf.values.clear();
            for (int i = 0; i < room_tree->children.size(); i++) {
                if (room_tree->children[i]->data == chonKhuTf.values[chonKhuTf.selected]) {
                    for (int j = 0; j < room_tree->children[i]->children.size(); j++) {
                        chonTangTf.values.push_back(room_tree->children[i]->children[j]->data);
                    }
                    break;
                }
            }
            chonTangTf.selected = 0;
        }

        void load_spinner_room() {
            chonPhongTf.values.clear();
            for (int i = 0; i < room_tree->children.size(); i++) {
                if (room_tree->children[i]->data == chonKhuTf.values[chonKhuTf.selected]) {
                    for (int j = 0; j < room_tree->children[i]->children.size(); j++) {
                        if (room_tree->children[i]->children[j]->data == chonTangTf.values[chonTangTf.selected]) {
                            for (int k = 0; k < room_tree->children[i]->children[j]->children.size(); k++) {
                                chonPhongTf.values.push_back(room_tree->children[i]->children[j]->children[k]->data);
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            chonPhongTf.selected = 0;
        }

        void update_spinner_block() {
            static string old_value = "";
            if (old_value != chonKhuTf.values[chonKhuTf.selected]) {
                old_value = chonKhuTf.values[chonKhuTf.selected];
                load_spinner_floor();
                load_spinner_room();
            }
        }

        void update_spinner_floor() {
            static string old_value = "";
            if (old_value != chonTangTf.values[chonTangTf.selected]) {
                old_value = chonTangTf.values[chonTangTf.selected];
                load_spinner_room();
            }
        }

        void init_student_n_room();

        void update_spinner_room();
};

