#ifndef PBL_APP_FACTORY_HPP
#define PBL_APP_FACTORY_HPP

#include "apps/App.hh"
#include "apps/AddStudent.hpp"
#include <memory>
#include <iostream>

enum class AppType {
    ADD_STUDENT,
    ROOM_RESERVATION,
    STUDENT_LIST,
    ROOM_LIST,
    MOVE_STUDENT,
    PRINT_DOCUMENT,
    PRINT_MAP,
    STATISTICS,
    EXIT,
};

class LabelFactory {
    public:
        static std::string produce(AppType type) {
            switch (type) {
                case AppType::ADD_STUDENT:
                    return "Thêm sinh viên";
                case AppType::ROOM_RESERVATION:
                    return "Đăng ký bao phòng";
                case AppType::STUDENT_LIST:
                    return "Danh sách sinh viên";
                case AppType::ROOM_LIST:
                    return "Danh sách phòng";
                case AppType::MOVE_STUDENT:
                    return "Chuyển sinh viên sang phòng khác";
                case AppType::PRINT_DOCUMENT:
                    return "In tài liệu (*.csv)";
                case AppType::PRINT_MAP:
                    return "In bản đồ";
                case AppType::STATISTICS:
                    return "Thống kê";
                case AppType::EXIT:
                    return "Thoát";
            }
            return "Xin chào";
        }
};

class DescriptionFactory {
    public:
        static std::string produce(AppType type) {
            switch (type) {
                case AppType::ADD_STUDENT:
                    return "add_student";
                case AppType::ROOM_RESERVATION:
                    return "room_reservation";
                case AppType::STUDENT_LIST:
                    return "student_list";
                case AppType::ROOM_LIST:
                    return "room_list";
                case AppType::MOVE_STUDENT:
                    return "move_student";
                case AppType::PRINT_DOCUMENT:
                    return "print_document";
                case AppType::PRINT_MAP:
                    return "print_map";
                case AppType::STATISTICS:
                    return "statistics";
                case AppType::EXIT:
                    return "exit";
            }
            return "";
        }
};

class MyExitApp : public App {
    public:
        virtual void run() {
            tui::stop();
        }
};

class AppFactory {
    public:
        static std::unique_ptr<App> produce(AppType type) {
            switch (type) {
                case AppType::ADD_STUDENT:
                    return std::make_unique<AddStudent>();
                case AppType::EXIT:
                    return std::make_unique<MyExitApp>();
            }
            return nullptr;
        }
};

#endif
