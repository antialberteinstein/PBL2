#pragma once

#include "apps/App.hh"

class Statistics : public App {
    public:
        Statistics();
        virtual ~Statistics() = default;
        virtual Element create_element() override;
        virtual bool event(Event event) override;
    private:

        string error_message;
};