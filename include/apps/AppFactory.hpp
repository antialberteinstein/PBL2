#ifndef PBL_APP_FACTORY_HPP
#define PBL_APP_FACTORY_HPP

#include "apps/App.hh"
#include "apps/AddStudent.hpp"
#include "apps/StudentList.hpp"
#include "apps/RoomList.hpp"
#include <memory>
#include <iostream>
#include "apps/PrintMap.hpp"
#include "apps/PrintDocument.hpp"
#include "apps/MoveStudent.hpp"
#include "apps/RoomReservation.hpp"
#include "apps/Statistics.hpp"

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
        static App* produce(AppType type) {
            switch (type) {
                case AppType::ADD_STUDENT:
                    return AppAdapter::connect(new AddStudent());
                case AppType::STUDENT_LIST:
                    return AppAdapter::connect(new StudentList());
                case AppType::ROOM_LIST:
                    return AppAdapter::connect(new RoomList());
                case AppType::PRINT_MAP:
                    return AppAdapter::connect(new PrintMap());
                case AppType::PRINT_DOCUMENT:
                    return AppAdapter::connect(new PrintDocument());
                case AppType::MOVE_STUDENT:
                    return AppAdapter::connect(new MoveStudent());
                case AppType::ROOM_RESERVATION:
                    return AppAdapter::connect(new RoomReservation());
                case AppType::STATISTICS:
                    return AppAdapter::connect(new Statistics());
                case AppType::EXIT:
                    return AppAdapter::connect(new MyExitApp());
            }
            return nullptr;
        }
};

#endif
