#include "eins/tui_support.h"
#include "apps/MainMenu.hpp"
#include "apps/AppFactory.hpp"
#include "models/Student.hpp"

#include "objects/Date.hpp"

#include "viewmodel/my_view_model.hpp"
#include "models/Room.hpp"
#include "eins/csv_creator.h"
#include "viewmodel/fee_calculator.hpp"
#include <thread>
#include "apps/SplashScreen.hpp"

bool pbl();

void recovery_rooms_database();

void test_csv_creator();

int main(void) {

    pbl();



    return 0;
}


bool pbl() {
    ModelProducer::init();

    tui::init();
    // FeeCalculator::init();

    auto splash_scr = Screen::Create(
        Dimension::Full(),
        Dimension::Full()
    );
    /* // Creating a thread.
    thread gauge_thread(process_gauge);

    thread ui_thread([&] {
        splash_screen::show();
        tui::start(screen);
    });

    gauge_thread.join();
    if (ui_thread.joinable()) {
        ui_thread.join();
    } */

    thread loading_data_thread([&] {
        FeeCalculator::init();
        splash_screen::flag_loaded_data();
    });

    thread splash_screen_thread([&] {
        splash_screen::run(splash_scr);
    });

    loading_data_thread.join();
    splash_screen_thread.join();

    auto screen = ScreenInteractive::TerminalOutput();

    tui::start(screen);

    // FeeCalculator::cleanup();
    tui::cleanup();

    ModelProducer::cleanup();

    return true;
}

void recovery_rooms_database() {
    auto room_db = ModelProducer::get_instance(ModelType::ROOM);
    char blocks[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    for (char block : blocks) {
        for (int floor = 1; floor <= 5; ++floor) {
            for (int room = 1; room <= 24; ++room) {
                auto room_ = make_unique<Room>();
                room_->set_block(string(1, block));
                room_->set_floor(floor);
                room_->set_room_number(room);
                room_->set_status(RoomStatus::AVAILABLE);
                room_->set_capacity(6);
                room_->set_current_number(0);
                room_db->add(room_.get());
            }
        }
    }
}

void test_csv_creator() {
    try {
        CSVCreator csv_creator;

        csv_creator.add_map("id");
        csv_creator.add_map("name");
        
        Vector<string> arr;
        arr.push_back("1");
        arr.push_back("Le Nha Phuong");

        csv_creator.add_record(arr);

        arr[0] = "2";
        arr[1] = "Tran Nhat Nguyen";

        csv_creator.add_record(arr);

        csv_creator.dump_to_file("test.csv");
    } catch (const string& s) {
        cout << s << endl;
    }
}
