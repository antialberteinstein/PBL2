#include "apps/RoomList.hpp"
#include "apps/MainMenu.hpp"
#include "viewmodel/my_view_model.hpp"


RoomList::RoomList() {
    will_render = true;

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

    // Lay danh sach phong tu database.

    scroller.add_map(ScrollerMap(), "id", "Mã số");
    scroller.add_map(ScrollerMap(), "name", "Tên phòng");
    scroller.add_map(ScrollerMap(), "current_number", "Số người hiện tại");
    scroller.add_map(ScrollerMap(), "status", "Tình trạng");

    init_db();
    
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
            Vector<string> record;
            record.push_back(room->get_id_string());
            record.push_back(room->get_string_name());
            record.push_back(to_string(room->get_current_number())
                    + "/"
                    + to_string(room->get_capacity()));
            record.push_back(room->get_status());

            scroller.add_record(record);
            scroller.update_visible_list();

            event_listener = Container::Vertical({
                search_com,
                info_btn,
                cancel_btn
            });
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
