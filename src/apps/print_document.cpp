#include "apps/print_document.hpp"

namespace print_document {
    Element create_element() {
        return text("Print document");
    }

    bool check_event(Event event) {
        return false;
    }

    void action() {
        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}