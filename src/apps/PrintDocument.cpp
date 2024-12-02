#include "apps/PrintDocument.hpp"
#include "apps/FileSelector.hpp"
#include "apps/MainMenu.hpp"
#include "viewmodel/my_view_model.hpp"
#include "models/Room.hpp"
#include "models/Student.hpp"
#include "eins/csv_creator.h"
#include <filesystem>


#define PM_OPTION_1 "In danh sách sinh viên (*.csv)"
#define PM_OPTION_2 "In danh sách phòng (*.csv)"

#define STUDENT_LIST_FILE_NAME "Danh_sach_sinh_vien"
#define ROOM_LIST_FILE_NAME "Danh_sach_phong"

string print_student_list(Vector<Student>& students, const string& path);

string print_room_list(Vector<Room>& rooms, const string& path);

PrintDocument::PrintDocument() {
    options.push_back(PM_OPTION_1);
    options.push_back(PM_OPTION_2);

    ddm_option_kinda_doc = Dropdown(
        StringAdapter::From(&options),
        &selected    
    );

    // Start to load data. 


    path = "";
    is_file_selected = false;
    choosing_file_btn = Button("Chọn file", [&]() {
        try {
            path = get_save_file_path();
            is_file_selected = true;
        } catch (const char* e) {
            error_message = e;
        }
    });

    print_btn = Button("In", [&]() {
        if (is_file_selected) {
            if (options[selected] == PM_OPTION_1) {
                Vector<Student> students;

                try {
                    auto student_db = ModelProducer::get_instance(ModelType::STUDENT);
                    if (student_db == nullptr) {
                        error_message = "Lỗi kết nối cơ sở dữ liệu!!";
                        return;
                    }
                    student_db->unflag_students_sorted();
                    auto keys = student_db->get_all_keys();
                    for (int i = 0; i < keys.size(); i++) {
                        auto student = student_db->get_student(keys[i]);
                        if (student) {
                            students.push_back(*student);
                        }
                    }


                    // Check if printed file is existed.
                    string base_output_path = path + "/"
                        + STUDENT_LIST_FILE_NAME;
                    string output_path_csv = base_output_path + ".csv";
                    int counter = 1;
                    while (filesystem::exists(output_path_csv)) {
                        output_path_csv = base_output_path + "(" + to_string(counter) + ").csv";
                        counter++;
                    }

                    error_message =
                        print_student_list(students, output_path_csv);
                } catch (const string& e) {
                    error_message = e;
                }
            }
            else if (options[selected] == PM_OPTION_2) {
                Vector<Room> rooms;

                try {
                    auto room_db = ModelProducer::get_instance(ModelType::ROOM);
                    if (room_db == nullptr) {
                        error_message = "Lỗi kết nối cơ sở dữ liệu!!";
                        return;
                    }
                    auto keys = room_db->get_all_keys();
                    for (int i = 0; i < keys.size(); i++) {
                        auto room = room_db->get_room(keys[i]);
                        if (room) {
                            rooms.push_back(*room);
                        }
                    }

                    // Check if printed file is existed.
                    string base_output_path = path + "/"
                        + ROOM_LIST_FILE_NAME;
                    string output_path_csv = base_output_path + ".csv";
                    int counter = 1;
                    while (filesystem::exists(output_path_csv)) {
                        output_path_csv = base_output_path + "(" + to_string(counter) + ").csv";
                        counter++;
                    }

                    error_message =
                        print_room_list(rooms, output_path_csv);
                } catch (const string& e) {
                    error_message = e;
                }
            }
        }
    });

    back_btn = Button("Quay lại", [&]() {
        is_file_selected = false;
        main_menu::show();
    });

    event_listener = Container::Vertical({
        ddm_option_kinda_doc,   
        choosing_file_btn,
        back_btn,
        print_btn,
    });
}

Element PrintDocument::create_element() {
    return vbox({
        get_title().get_doc() | color(TITLE_COLOR),
        separator(),
        ddm_option_kinda_doc->Render() | center | flex,
        hbox({
            choosing_file_btn->Render() | center | flex,
            back_btn->Render() | center | flex,
        }) | center | flex,
        text(path) | center | border,
        error_message.empty() ? text("") : text(error_message) | center | color(ERROR_COLOR),
        is_file_selected ? print_btn->Render() | center | flex : text(""),
    });
}

bool PrintDocument::event(Event event) {
    return event_listener->OnEvent(event);
}


string print_student_list(Vector<Student>& students, const string& path) {
    /*
        List of fields:
            - ID
            - Name
            - Date of birth
            - Phone number
            - Hometown
            - Email
            - Gender
            - University
            - Major
            - Date joined
            - Room
    */

    try {
        CSVCreator csv_creator;
        csv_creator.add_map("Mã số");
        csv_creator.add_map("Họ và tên");
        csv_creator.add_map("Ngày sinh");
        csv_creator.add_map("Số điện thoại");
        csv_creator.add_map("Quê quán");
        csv_creator.add_map("Email");
        csv_creator.add_map("Giới tính");
        csv_creator.add_map("Trường");
        csv_creator.add_map("Ngành học");
        csv_creator.add_map("Ngày vào ở");
        csv_creator.add_map("Phòng");

        for (int i = 0; i < students.size(); ++i) {
            Vector<string> arr;
            arr.push_back("'" + students[i].get_id());
            arr.push_back(students[i].get_name());
            arr.push_back(students[i].get_dob());
            arr.push_back("'" + students[i].get_phone_number());
            arr.push_back(students[i].get_hometown());
            arr.push_back(students[i].get_email());
            arr.push_back(students[i].get_gender());
            arr.push_back(students[i].get_university());
            arr.push_back(students[i].get_major());
            arr.push_back(students[i].get_date_joined());
            arr.push_back(students[i].get_room());

            csv_creator.add_record(arr);
        }

        csv_creator.dump_to_file(path);

        return "Đã in xong.";
    } catch (const string& s) {
        return s;
    } catch (...) {
        return "Lỗi không xác định.";
    }
}

string print_room_list(Vector<Room>& rooms, const string& path) {
    /*
        List of fields:
            - ID
            - Block
            - Floor
            - Room number
            - Capacity
            - Current number
            - Status
    */
    try {
        CSVCreator csv_creator;
        csv_creator.add_map("Mã phòng");
        csv_creator.add_map("Khu");
        csv_creator.add_map("Tầng");
        csv_creator.add_map("Số phòng");
        csv_creator.add_map("Số người tối đa");
        csv_creator.add_map("Số người hiện tại");
        csv_creator.add_map("Tình trạng");

        for (int i = 0; i < rooms.size(); ++i) {
            Vector<string> arr;
            arr.push_back(rooms[i].get_id());
            arr.push_back(rooms[i].get_block());
            arr.push_back(to_string(rooms[i].get_floor()));
            arr.push_back(to_string(rooms[i].get_room_number()));
            arr.push_back(to_string(rooms[i].get_capacity()));
            arr.push_back(to_string(rooms[i].get_current_number()));
            arr.push_back(rooms[i].get_status());

            csv_creator.add_record(arr);
        }

        csv_creator.dump_to_file(path);

        return "Đã in xong.";
    } catch (const string& s) {
        return s;
    } catch (...) {
        return "Lỗi không xác định.";
    }

}
