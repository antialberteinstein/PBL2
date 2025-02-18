#include "apps/RoomList.hpp"
#include "apps/MainMenu.hpp"
#include "viewmodel/my_view_model.hpp"
#include "viewmodel/fee_calculator.hpp"

RoomList::RoomList() {
    will_render = true;
    filter_flag_electricity = false;
    filter_flag_maintainance = false;

    try {
        room_db = ModelProducer::get_instance(ModelType::ROOM);
    } catch (const string& e) {
        try {
            room_db = ModelProducer::get_instance(ModelType::ROOM);
        } catch (const string& e) {
            error_message = "Lỗi kết nối cơ sở dữ liệu!!";
            will_render = false;
        }
    }

    search_com = Input(&search_string, "Tìm kiếm");

    info_btn = Button("Thông tin chi tiết", [&] {
        string key = scroller.get_current_item()[0];
        auto room = room_db->get_room(key);
        if (room == nullptr) {
            error_message = "Lỗi kết nối cơ sở dữ liệu!!";
            return;
        }

        this->detail = make_unique<RoomDetail>(this, move(room));
        if (detail == nullptr) {
            error_message = "Không thể xem thông tin chi tiết!!";
            return;
        } else {
            detail->run();
        }


    }, ButtonOption::Animated(CONFIRM_BTN_BG));

    cancel_btn = Button("Trở về", [&] {
        main_menu::show();
    }, ButtonOption::Animated(CANCEL_BTN_BG));

    filter_btn_maintainance = Button("Hiển thị phòng đang bảo trì/ Hiện tất cả", [&] {
        if (filter_flag_maintainance) {
            flag_not_filter_maintainance();
        } else {
            flag_filter_maintainance();
        }
    }, ButtonOption::Animated(Color::DeepSkyBlue2));

    filter_btn_electricity = Button("Hiển thị phòng chưa thanh toán tiền điện/ Hiện tất cả", [&] {
        if (filter_flag_electricity) {
            flag_not_filter_electricity();
        } else {
            flag_filter_electricity();
        }
    }, ButtonOption::Animated(Color::DeepSkyBlue1));

    // Lay danh sach phong tu database.

    scroller.add_map(ScrollerMap(), "id", "Mã số");
    scroller.add_map(ScrollerMap(), "name", "Tên phòng");
    scroller.add_map(ScrollerMap(), "current_number", "Số người hiện tại");
    scroller.add_map(ScrollerMap(), "status", "Tình trạng");
    scroller.add_map(ScrollerMap(), "fee", "Tiền điện còn thiếu");

    init_db();
    
    event_listener = Container::Vertical({
        search_com,
        info_btn,
        cancel_btn,
        filter_btn_maintainance,
        filter_btn_electricity,
    });
}

void RoomList::init_db() {
    scroller.clear();
    if (room_db == nullptr) {
        error_message = "Lỗi kết nối cơ sở dữ liệu!!";
    } else {
        Vector<string> keys = room_db->get_all_keys();
        for (int i = 0; i < keys.size(); i++) {
            auto room = room_db->get_room(keys[i]);
            if (room == nullptr) {
                continue;
            }

            if (filter_flag_maintainance && room->get_status() != RoomStatus::MAINTENANCE) {
                continue;
            }

            if (filter_flag_electricity) {
                try {
                    auto ef_db = FeeCalculator::get_instance(FeeType::ELECTRICITY_FEE);
                    if (ef_db != nullptr) {
                        auto ef_payment = ef_db->get_payment(room.get());
                        if (ef_payment != nullptr && ef_payment->get_status() == PaymentStatus::PAID) {
                            continue;
                        }
                    }
                } catch (...) {
                    // Do nothing.
                }
            }

            Vector<string> record;
            record.push_back(room->get_id_string());
            record.push_back(room->get_string_name());
            record.push_back(to_string(room->get_current_number())
                    + "/"
                    + to_string(room->get_capacity()));
            record.push_back(room->get_status());
            string fee = "Không có";
            try {
                auto ef_db = FeeCalculator::get_instance(FeeType::ELECTRICITY_FEE);
                if (ef_db != nullptr) {
                    auto ef_payment = ef_db->get_payment(room.get());
                    if (ef_payment != nullptr) {
                        fee = to_string(ef_payment->get_amount()) + " VND";
                    }
                }
            } catch (...) {
                // Do nothing.
            }
            record.push_back(fee);

            scroller.add_record(record);
            scroller.update_visible_list();
        }
    }
    scroller.update_visible_list();
}


Element RoomList::create_element() {
    // Neu khong co text thi reset lai danh sach phong.
    if (search_string.empty()) {
        scroller.filter_reset();
    }

    // Neu co text change thi filter lai danh sach phong.
    if (search_string != backup_search_string) {
        scroller.filter_linear("name", search_string);
        backup_search_string = search_string;
    }

    auto title_box = get_title().get_doc() | color(TITLE_COLOR);

    if (!will_render) {
        return vbox({
            title_box,
            separator(),
            text(error_message) | center | color(ERROR_COLOR),
            cancel_btn->Render() | center | flex,
        });
    }

    return vbox({
        title_box,
        separator(),
        text(error_message) | center | color(ERROR_COLOR),
        search_com->Render() | center | flex,
        hbox({
            info_btn->Render() | center | flex,
            filter_btn_maintainance->Render() | center | flex,
            filter_btn_electricity->Render() | center | flex,
            cancel_btn->Render() | center | flex,
        }),
        scroller.Render(),
    });
}

bool RoomList::event(Event event) {

    if (event == Event::Return) {
        // Prevent read the return event for the search input.

        return info_btn->OnEvent(event);;
    }

    if (event.is_character() || event == Event::Backspace) {
        bool is_search = search_com->OnEvent(event);

        // Xu li tim kiem phong.
        // Neu khong co text thi reset lai danh sach phong.

        return is_search;
    } else if (event == Event::ArrowDown || event == Event::ArrowUp) {

        return scroller.OnEvent(event);
    } 

    return event_listener->OnEvent(event);
}
