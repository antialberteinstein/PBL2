#include "eins/tui_support.h"
#include "apps/MainMenu.hpp"
#include "apps/AppFactory.hpp"
#include "models/Student.hpp"

#include "objects/Date.hpp"

#include "viewmodel/my_view_model.hpp"
#include "models/Room.hpp"
#include "eins/csv_creator.h"

bool pbl();

void recovery_rooms_database();

void test_csv_creator();

int main(void) {
    ModelProducer::init();

    pbl();

    ModelProducer::cleanup();

    return 0;
}



bool pbl() {
     {
    tui::init();
    auto screen = ScreenInteractive::TerminalOutput();

    main_menu::show();

    tui::start(screen);

    tui::cleanup();

    return true;
}
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
