#include "apps/statistics.hpp"

namespace statistics {
    Element create_element() {
        return text("Statistics");
    }

    bool check_event(Event event) {
        return false;
    }

    void action() {
        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}