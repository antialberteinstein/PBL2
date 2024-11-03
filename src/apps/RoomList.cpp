#include "apps/RoomList.hpp"
#include "apps/MainMenu.hpp"
#include "viewmodel/my_view_model.hpp"


RoomList::RoomList() {
    will_render = true;

    try {
        room_db = ModelProducer::get_instance(ModelType::ROOM);
    } catch (const runtime_error& e) {
        try {
            room_db = ModelProducer::get_instance(ModelType::ROOM);
        } catch (const runtime_error& e) {
            error_message = "Lỗi kết nối cơ sở dữ liệu!!";
            will_render = false;
        }
    }

    search_com = Input(&search_string, "Tìm kiếm");

    info_btn = Button("Thông tin chi tiết", [&] {
        // Do nothing at this moment.
    }, ButtonOption::Animated(CONFIRM_BTN_BG));

    cancel_btn = Button("Trở về", [&] {
        main_menu::show();
    }, ButtonOption::Animated(CANCEL_BTN_BG));

    // Lay danh sach phong tu database.

    scroller.add_map(ScrollerMap(), "id", "Mã số");
    scroller.add_map(ScrollerMap(), "name", "Tên phòng");
    scroller.add_map(ScrollerMap(), "current_number", "Số người hiện tại");
    scroller.add_map(ScrollerMap(), "status", "Tình trạng");

    if (room_db == nullptr) {
        error_message = "Lỗi kết nối cơ sở dữ liệu!!";
    } else {
        for (int i = 1; i <= room_db->size(); i++) {
            try {
                auto room = room_db->get_room(to_string(i));
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
            } catch (const runtime_error& e) {
                error_message = "Lỗi kết nối cơ sở dữ liệu!!";
                will_render = false;
                break;
            }

            scroller.update_visible_list();

            event_listener = Container::Vertical({
                search_com,
                info_btn,
                cancel_btn
            });
        }
    }
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

        return true;
    }

    if (event.is_character()) {
        bool is_search = search_com->OnEvent(event);

        // Xu li tim kiem phong.
        // Neu khong co text thi reset lai danh sach phong.

        return is_search;
    } else if (event == Event::ArrowDown || event == Event::ArrowUp) {

        return scroller.OnEvent(event);
    } 

    return event_listener->OnEvent(event) || scroller.OnMouseEvent(event);
}
