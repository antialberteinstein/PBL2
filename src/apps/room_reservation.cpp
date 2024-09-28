#include "apps/room_reservation.hpp"

namespace room_reservation {
    Element create_element() {
        return text("Room reservation");
    }

    bool check_event(Event event) {
        return false;
    }

    void action() {
        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}