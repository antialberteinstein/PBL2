#include "apps/room_list.hpp"

namespace room_list {
    Element create_element() {
        return text("Room list");
    }

    bool check_event(Event event) {
        return false;
    }

    void action() {
        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}