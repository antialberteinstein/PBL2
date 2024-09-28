#include "apps/print_map.hpp"

namespace print_map {
    Element create_element() {
        return text("Print map");
    }

    bool check_event(Event event) {
        return false;
    }

    void action() {
        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}