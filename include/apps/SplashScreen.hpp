#ifndef PBL_SPLASH_SCREEN_HPP
#define PBL_SPLASH_SCREEN_HPP

#include "apps/App.hh"

#define PERIOD 300  // microseconds (ms)
#define DEFAULT_SPEED 2.5 // per period.
#define GROUP_NAME_PATH "res/ascii_art/group_name.txt"
#define GROUP_NAME_TEXT "NHOM 12"


class SplashScreen : public App {
    public:
        SplashScreen();
        virtual Element create_element() override;

        void perform();

        virtual bool event(Event event) override;

        void set_progress(float progress);

        void set_speed(float speed) {
            this->speed = speed;
        }

        bool is_complete() {
            return progress >= 100;
        }
    private:
        string progress_text;

        float progress;
        float speed;  // per 60ms
};

namespace splash_screen {

    void set_progress(float progress);

    void set_speed(float speed);

    bool is_complete();

    void run(Screen& screen);

    void flag_loaded_data();

    void flag_not_loaded_data();
}



#endif