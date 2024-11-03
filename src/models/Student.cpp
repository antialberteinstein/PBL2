#include "models/Student.hpp"
#include "viewmodel/my_view_model.hpp"
#include "models/Room.hpp"

string Student::get_room() {
    if (room_id == -1) {
        return "Chưa được phân phòng";
    }
    try {
        auto room_db = ModelProducer::get_instance(ModelType::ROOM);
        unique_ptr<Room> room = room_db->get_room(to_string(room_id));
        return room->get_string_name();
    } catch (const runtime_error& e) {
        return "Lỗi kết nối cơ sở dữ liệu";
    }
}