#include "apps/StudentList.hh"

StudentList::~StudentList() {
    // Do nothing
}

StudentList::StudentList() {
    return_btn = Button("Quay lại", [] {
        // Do later/
    });
}

Element StudentList::create_element() {
    return vbox({
        get_title().get_doc() | color(TITLE_COLOR),
        separator(),
        text(error_message) | center | color(RED),
        menu_list->Render() | border | flex,
    });
}
