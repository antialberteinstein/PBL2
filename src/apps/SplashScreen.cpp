#include "apps/SplashScreen.hpp"
#include <thread>
#include <chrono>
#include "apps/MainMenu.hpp"

Title __group_name__(GROUP_NAME_PATH, GROUP_NAME_TEXT);

#define EVENT_CALLER Event::Return

const string progress_texts[] = {
    "Đang tải cơ sở dữ liệu...",
    "Tải dữ liệu sinh viên...",
    "Tải dữ liệu phòng...",
    "Tính toán tiền điện...",
    "Tính toán tiền phòng...",
    "Đã xong, vui lòng chờ..."
};

const float progress_values[] = {
    40,
    45,
    55,
    80,
    99,
    100
};

const float progress_speeds[] = {
    DEFAULT_SPEED * 2,
    DEFAULT_SPEED,
    DEFAULT_SPEED * 0.8,
    DEFAULT_SPEED * 0.6,
    DEFAULT_SPEED * 0.4,
    DEFAULT_SPEED * 0.2,
};

void _delay(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void SplashScreen::set_progress(float progress) {
    this->progress = progress;
}


SplashScreen::SplashScreen() {
    progress_text = progress_texts[0];
    progress = 0;
    speed = progress_speeds[0];
}


Element SplashScreen::create_element() {
    static int index = 0;
    progress += speed;
    if (index < 5 && progress >= progress_values[index]) {
        progress_text = progress_texts[++index];
        speed = progress_speeds[index];
    }

    float _progress = progress/100;
    if (_progress > 1) {
        _progress = 1;
    }
    return vbox({
        __group_name__.get_doc() | color(TITLE_COLOR),
        separator(),
        vbox({
            text("Giới thiệu:"),
            text(""),
            text("Ứng dụng quản lý Kí túc xá phiên bản 1.0.0 - Đồ án 2 : Cơ sở lập trình - Khoa CNTT, Đại học Bách Khoa - Đại học Đà Nẵng."),
            text("Địa chỉ: Kí túc xá Phía Tây Thành Phố Đà Nẵng, 08 Hà Văn Tính, Hoà Khánh Nam, Liên Chiểu, Đà Nẵng."),
            text("Ứng dụng này được tạo trên thư viện đồ hoạ nổi tiếng FTXUI (ArthurSonzoghi/FTXUI), hệ quản trị cơ sở dữ liệu leveldb trên nền tảng ngôn ngữ lập trình C++."),
            text("Ứng dụng đang trong quá trình phát triển, source code: https://github.com/antialberteinstein/PBL2.git"),
        }) | border | color(Color::White),
        vbox({
            text("Về chúng tôi:"),
            text(""),
            text("Trần Nhật Nguyên - 102230203 - 23T_DT1 - 23.Nh10B"),
            text("Trần Viết Tâm - 102230100 - 23T_Nhat2 - 23.Nh10B"),
        }) | border | color(Color::White) | bgcolor(Color::DarkGreen) | bold,
        separator(),
        text(progress_text) | center | color(Color::White),
        border(gaugeRight(_progress)) | color(Color::DeepSkyBlue2),

    });
}

void SplashScreen::perform() {}
    

bool SplashScreen::event(Event event) {
    // Check if progress is complete.
    if (is_complete()) {
        main_menu::show();
    }

    progress += speed;
    return true;
}

namespace splash_screen {
    SplashScreen _splash_screen;
    Component renderer;

    bool loaded_data_flag = false;

    void set_progress(float progress) {
        _splash_screen.set_progress(progress);
    }

    bool is_complete() {
        return _splash_screen.is_complete();
    }

    void set_speed(float speed) {
        _splash_screen.set_speed(speed);
    }

    void flag_loaded_data() {
        loaded_data_flag = true;
    }

    void flag_not_loaded_data() {
        loaded_data_flag = false;
    }

    void run(Screen& screen) {
        while (!_splash_screen.is_complete() && !loaded_data_flag) {
            clear_screen();
            Render(screen, _splash_screen.create_element());
            screen.Print();
            _delay(PERIOD);
        }

        main_menu::show();

        
    }
}