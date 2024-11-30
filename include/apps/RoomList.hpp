#ifndef PBL_ROOM_LIST_HPP
#define PBL_ROOM_LIST_HPP

#include "apps/App.hh"
#include "objects/Vector.hpp"
#include "objects/StringAdapter.hpp"
#include "apps/Scroller.hpp"
#include "apps/RoomDetail.hpp"

void debug(const string& s);

class ModelProducer;

class RoomList : public App {
    public:
        RoomList();
        virtual ~RoomList() = default;
        virtual Element create_element() override;
        virtual bool event(Event event) override;

        void init_db();
    private:
        Scroller scroller;
        string error_message;
        bool will_render;  // If false, render error message only
        ModelProducer* room_db;
        Component search_com;
        string search_string;
        string backup_search_string;
        Component info_btn, cancel_btn;
        Component event_listener;

        // Hien thi phong dang bao tri.
        bool filter_flag_maintainance;
        Component filter_btn_maintainance;

        // Hien thi phong chua thanh toan tien dien.
        bool filter_flag_electricity;
        Component filter_btn_electricity;

        unique_ptr<RoomDetail> detail;

        void flag_filter_maintainance() {
            filter_flag_maintainance = true;
            init_db();
        }

        void flag_not_filter_maintainance() {
            filter_flag_maintainance = false;
            init_db();
        }

        void flag_filter_electricity() {
            filter_flag_electricity = true;
            init_db();
        }

        void flag_not_filter_electricity() {
            filter_flag_electricity = false;
            init_db();
        }
};

#endif // PBL_ROOM_LIST_HPP