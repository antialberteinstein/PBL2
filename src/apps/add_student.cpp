#include "apps/add_student.hpp"

namespace add_student {

    Form form(20);

    Element create_element() {
        auto title_box = get_title().get_doc() | color(TITLE_COLOR);
        auto name_input = text("");
        
        form.add_text_field("CCCD");
        form.add_text_field("Họ và tên");
        form.add_text_field("Ngày sinh");
        form.add_text_field("Giới tính");
        form.add_text_field("Quê quán");
        form.add_text_field("Trường");
        form.add_text_field("Ngành học");
        form.add_text_field("Số diện thoại");
        form.add_text_field("Email");

        return vbox({
            title_box,
            separator(),
            form.get_doc(),
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
