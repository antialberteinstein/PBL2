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

        unique_ptr<RoomDetail> detail;
};

#endif // PBL_ROOM_LIST_HPP