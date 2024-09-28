#include "apps/student_list.hpp"

namespace student_list {
    Element create_element() {
        auto title_box = get_title().get_doc() | color(TITLE_COLOR);
        return vbox({
            title_box,
            text("Student list") | flex | border,
        });
    }

    bool check_event(Event event) {
        return false;
    }

    void action() {
        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}