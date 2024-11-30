#include "apps/Statistics.hpp"

Statistics::Statistics() {
    //
}

Element Statistics::create_element() {
    return text("no render");
}

bool Statistics::event(Event event) {
    return false;
}