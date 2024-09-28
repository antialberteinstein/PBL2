#include "apps/add_student.hpp"

namespace add_student {

    Form form;

    Element create_element() {
        auto title_box = get_title().get_doc() | color(TITLE_COLOR);
        auto name_input = text("");
        
        form.add_text_field("Họ và tên");
        form.add_text_field("Trường");

        return vbox({
            title_box,
            separator(),
            form.get_doc() | flex | border,
        });
    }

    bool check_event(Event event) {
        return form.check_event(event);
    }

    void action() {
        set_current_render_element(create_element);
        set_event_listener(check_event);
    }
}
