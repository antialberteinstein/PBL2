#include "apps/StudentList.hpp"
#include "apps/MainMenu.hpp"
#include "viewmodel/my_view_model.hpp"

StudentList::StudentList() {
    will_render = true;
    filter_flag = false;

    filter_btn = Button("Hiển thị sinh viên nợ tiền phòng/ Hiện tất cả", [&] {
        if (filter_flag) {
            flag_not_filter();
        } else {
            flag_filter();
        }
    }, ButtonOption::Animated(Color::DeepSkyBlue2));

    try {
        student_db = ModelProducer::get_instance(ModelType::STUDENT);
    } catch (const string& msg) {
        try {
            student_db = ModelProducer::get_instance(ModelType::STUDENT);
        } catch (const string& msg) {
            error_message = "Lỗi kết nối cơ sở dữ liệu!!";
            will_render = false;
        }
    }

    search_com = Input(&search_string, "Tìm kiếm");

    info_btn = Button("Thông tin chi tiết", [&] {
        string key = scroller.get_current_item()[0];
        auto student = student_db->get_student(key);
        if (student == nullptr) {
            error_message = "Lỗi kết nối cơ sở dữ liệu!!";
            return;
        }

        this->detail = make_unique<StudentDetail>(this, move(student));
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


    // Lay danh sach sinh vien tu database.

    scroller.add_map(ScrollerMap(), "id", "Mã số");
    scroller.add_map(ScrollerMap(), "name", "Họ và tên");
    scroller.add_map(ScrollerMap(), "gender", "Giới tính");
    scroller.add_map(ScrollerMap(), "dob", "Ngày sinh");
    scroller.add_map(ScrollerMap(), "hometown", "Quê quán");
    scroller.add_map(ScrollerMap(), "university", "Trường");
    scroller.add_map(ScrollerMap(), "room", "Phòng");

    init_db();

    event_listener = Container::Vertical({
        search_com,
        info_btn,
        cancel_btn,
        filter_btn,
    });
}

void StudentList::init_db() {
    scroller.clear();
    if (student_db == nullptr) {
        error_message = "Lỗi kết nối cơ sở dữ liệu!!";
    } else {
        student_db->unflag_students_sorted();
        Vector<string> keys = student_db->get_all_keys();
        for (int i = 0; i < keys.size(); i++) {
            try {
                auto student = student_db->get_student(keys[i]);
                if (student == nullptr) {
                    continue;
                }
                if (filter_flag) {
                    try {
                        auto room_fee_db = ModelProducer::get_instance(ModelType::ROOM_FEE_PAYMENT);
                        if (room_fee_db) {
                            auto room_fee = room_fee_db->get_room_fee_payment(student->get_id_string());
                            if (room_fee == nullptr) {
                                continue;
                            }
                            if (room_fee->get_status() == PaymentStatus::PAID) {
                                continue;
                            }
                        }
                    } catch (const string& msg) {
                        error_message = msg;
                        break;
                    }
                }

                Vector<string> record;
                record.push_back(student->get_id_string());
                record.push_back(student->get_name());
                record.push_back(student->get_gender());
                record.push_back(student->get_dob());
                record.push_back(student->get_hometown());
                record.push_back(student->get_university());
                record.push_back(student->get_room());

                scroller.add_record(record);
            } catch (const string& msg) {
                error_message = msg;
                break;
            } catch (exception& e) {
                error_message = e.what();
                break;
            } catch (...) {
                error_message = "Lỗi không xác định.";
                break;    
            }
        }
    }

    scroller.update_visible_list();
}

Element StudentList::create_element() {

    // Neu khong co text thi reset lai danh sach sinh vien.
    if (search_string.empty()) {
        scroller.filter_reset();
    }

    // Neu co text change thi filter lai danh sach sinh vien.
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
        text(""),  // Empty line
        hbox({
            info_btn->Render() | center | flex,
            filter_btn->Render() | center | flex,
            cancel_btn->Render() | center | flex,
        }),
        scroller.Render(),
    });
}

bool StudentList::event(Event event) {

    if (event == Event::Return) {
        // Prevent read the return event for the search input.
        
        return true;
    }

    if (event.is_character() || event == Event::Backspace) {
        bool is_search = search_com->OnEvent(event);

        // Xu li tim kiem sinh vien.
        // Neu khong co text thi reset lai danh sach sinh vien.

        return is_search;
    } else if (event == Event::ArrowDown || event == Event::ArrowUp) {

        return scroller.OnEvent(event);
    } 

    return event_listener->OnEvent(event);
}

