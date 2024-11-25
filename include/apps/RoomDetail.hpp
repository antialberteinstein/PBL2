#ifndef PBL_ROOM_DETAIL_HPP
#define PBL_ROOM_DETAIL_HPP

#include "apps/App.hh"
#include "models/Room.hpp"

class RoomDetail : public App {
    public:
        RoomDetail(App* parent, unique_ptr<Room> room);
        Element create_element() override;
        bool event(Event) override;
    private:
        unique_ptr<Room> room;
        Component payment_btn;  // Thanh toan.
        Component return_btn;  // Quay lai.
        Component notify_maintain_btn;  // Thong bao sua chua.
        Component register_btn;  // Dang ky bao phong.

        string error_message;

        App* parent;
};

#endif