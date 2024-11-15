#include "models/Student.hpp"
#include "viewmodel/my_view_model.hpp"
#include "models/Room.hpp"

string Student::get_room() {
    if (room_id == "") {
        return "Chưa được phân phòng";
    }
    try {
        auto room_db = ModelProducer::get_instance(ModelType::ROOM);
        unique_ptr<Room> room = room_db->get_room(room_id);
        return room->get_string_name();
    } catch (const string& e) {
        return "Lỗi kết nối cơ sở dữ liệu";
    }
}

unique_ptr<Room> Student::get_room_ref() {
    if (room_id == "") {
        return nullptr;
    }
    try {
        auto room_db = ModelProducer::get_instance(ModelType::ROOM);
        return room_db->get_room(room_id);
    } catch (const string& e) {
        return nullptr;
    }
}

string Student::hash_to_id() {
    if (creating_flag) {
        auto db = ModelProducer::get_instance(ModelType::STUDENT);
        string _id = to_string(db->size());
        if (_id.size() < 6) {
            _id = string(6 - _id.size(), '0') + _id;
        }
        return _id;
    }
    if (id == "") {
        unsigned long long hash = 0;
        for (char c : name) {
            hash = (hash * 31 + c) % 1000000007;
        }
        return to_string(hash);
    }
    return id;
}
