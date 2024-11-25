#pragma once

#include "apps/App.hh"
#include "apps/components.hpp"
#include "models/Room.hpp"

class ModelProducer;

class RoomReservation : public App {
    public:
        RoomReservation(App* parent=nullptr, string room_id="");
        virtual ~RoomReservation() = default;
        virtual Element create_element() override;
        virtual bool event(Event event) override;
    private:

        EditText maPhongTf;

        EditText soLuongTf;

        Component confirm_btn;
        Component return_btn;

        ModelProducer* room_db;
        App* parent;
        string room_id;
        string old_room_id;  // For text change event.
        unique_ptr<Room> room;

        string error_message;
        Component event_listener;

        void init_room();
};