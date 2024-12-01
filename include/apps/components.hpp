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

struct DonutChart {
    Vector<string> labels;
    Vector<int> values;
    Vector<Color> colors;

    float total;
    Canvas canvas;

    DonutChart(
        Vector<string> labels,
        Vector<int> values,
        Vector<Color> colors)
        : labels(labels), values(values), colors(colors) {
            canvas = Canvas(100, 100);
            for (int i = 0; i < values.size(); i++) {
                total += values[i];
            }
        }
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
