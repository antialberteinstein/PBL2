#include "apps/RoomReservation.hpp"
#include "apps/MainMenu.hpp"
#include "viewmodel/my_view_model.hpp"
#include "apps/RoomList.hpp"

RoomReservation::RoomReservation(App* parent, string room_id) {
    error_message = "";
    this->parent = parent;
    this->old_room_id = room_id;
    this->room_id = room_id;

    maPhongTf = {"Mã phòng", room_id, Input(&maPhongTf.value, "")};
    soLuongTf = {"Số lượng sinh viên ở phòng bao", "", Input(&soLuongTf.value, "")};

    room_db = nullptr;
    room = nullptr;

    confirm_btn = Button("Xác nhận", [&] {
        if (room != nullptr) {
            int needed_value = 6;
            try {
                needed_value = stoi(soLuongTf.value);
            } catch (...) {
                error_message = "Số lượng sinh viên không hợp lệ";
                return;
            }

            if (needed_value <= 0 || needed_value > 6) {
                error_message = "Số lượng sinh viên không hợp lệ (1-6)";
                return;
            }

            if (needed_value < room->get_current_number()) {
                error_message = "Số lượng sinh viên không thể nhỏ hơn số sinh viên hiện tại";
                return;
            }

            room->set_capacity(needed_value);
            try {
                if (this->room_db) {
                    this->room_db->modify(room->get_id(), room.get());
                }
            } catch (const string& msg) {
                error_message = "Lỗi kết nối cơ sở dữ liệu!!";
            }
            if (this->parent != nullptr) {

                auto temp = dynamic_cast<RoomList*>(this->parent);
                if (temp != nullptr) {
                    temp->init_db();
                }

                this->parent->run();
            } else {
                main_menu::show();
            }
        } else {
            error_message = "Không tìm thấy phòng";
        }
    }, ButtonOption::Animated(CONFIRM_BTN_BG));

    return_btn = Button("Quay lại", [&] {
        if (this->parent != nullptr) {
            this->parent->run();
        } else {
            main_menu::show();
        }
    }, ButtonOption::Animated(CANCEL_BTN_BG));

    try {
        room_db = ModelProducer::get_instance(ModelType::ROOM);
    } catch (const string& msg) {
        error_message = msg;
    } catch (...) {
        error_message = "Lỗi kết nối cơ sở dữ liệu!!";
    }


    init_room();

    event_listener = Container::Vertical({
        maPhongTf.com,
        soLuongTf.com,
        confirm_btn,
        return_btn,
    });
}

void RoomReservation::init_room() {
    if (room_db) {
        room = room_db->get_room(room_id);
    }
}

Element RoomReservation::create_element() {
    auto room_detail = text("Không có thông tin phòng");
    if (room != nullptr) {
        room_detail = vbox({
            text("Phòng: " + room->get_id_string()),
            text("Sức chứa: " + to_string(room->get_capacity())),
            text("Số sinh viên hiện tại: " + to_string(room->get_current_number())),
            text("Trạng thái: " + room->get_status()),
        }) | border | flex;
    }

    return vbox({
        get_title().get_doc() | color(TITLE_COLOR),
        separator(),
        text(error_message) | center | color(ERROR_COLOR),
        hbox({
            vbox({
                get_doc(maPhongTf),
                room_detail | flex | border,
            }) | flex,
            vbox({
                get_doc(soLuongTf),
                confirm_btn->Render(),
                return_btn->Render(),
            }) | flex,
        }),
    });
}

bool RoomReservation::event(Event event) {
    if (old_room_id != maPhongTf.value) {
        old_room_id = room_id;
        room_id = maPhongTf.value;
        init_room();
    }

    return event_listener->OnEvent(event);
}