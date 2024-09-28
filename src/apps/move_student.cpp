#include "apps/move_student.hpp"

namespace move_student {
    Element create_element() {
        return text("Move student");
    }

    bool check_event(Event event) {
        return false;
    }

    void action() {
        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}