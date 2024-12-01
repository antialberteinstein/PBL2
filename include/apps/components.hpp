#pragma once

#include "apps/App.hh"
#include "objects/Vector.hpp"

struct EditText {
    string label;
    string value;
    Component com;
};

struct ComboBox {
    string label;
    Vector<string> values;
    int selected;
    Component com;
};

inline Element get_doc(EditText& field) {
    return hbox({
        text(field.label),
        separator(),
        text(INPUT_PADDING),
        field.com->Render() | inverted,
    }) | border;
}

inline Element get_doc(ComboBox& field) {
    return hbox({
        text(field.label) | border,
        text(INPUT_PADDING),
        field.com->Render() | flex,
    });
}
