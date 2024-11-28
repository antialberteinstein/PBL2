#include "models/Student.hpp"
#include "viewmodel/my_view_model.hpp"
#include "models/Room.hpp"

int Student::number_of_students = 0;
string Student::room_id_to_fit = "";

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

Vector<Student> Student::get_students_living_in(string room_id) {
    Vector<Student> students;
    auto student_db = ModelProducer::get_instance(ModelType::STUDENT);
    auto keys = student_db->get_all_keys();
    for (int i = 0; i < keys.size(); i++) {
        auto student = student_db->get_student(keys[i]);
        if (student == nullptr) {
            continue;
        }
        if (student->get_room_id() == room_id) {
            students.push_back(*student);
        }
    }

    room_id_to_fit = room_id;
    number_of_students = students.size();

    return students;
}

void Student::fit_room(string room_id) {
    auto room_db = ModelProducer::get_instance(ModelType::ROOM);
    if (room_db != nullptr) {
        if (room_id_to_fit != room_id) {
            Vector<Student> students = get_students_living_in(room_id);
            number_of_students = students.size();
            room_id_to_fit = room_id;
        }
        auto room = room_db->get_room(room_id);
        if (room) {
            room->set_current_number(number_of_students);
            if (room->get_current_number() == room->get_capacity()) {
                room->set_status(RoomStatus::FULL);
            } else if (room->get_current_number() < room->get_capacity()) {
                room->set_status(RoomStatus::AVAILABLE);
            }
            room_db->modify(room->get_id(), room.get());
        }
    }
}


// Define a trigger.
void Student::on_modify() {
    if (modified_flag) {
        modified_flag = false;

        try {
            auto room_db = ModelProducer::get_instance(ModelType::ROOM);
            if (room_db) {
                auto room = room_db->get_room(room_id);
                if (room) {
                    if (room->get_status() == RoomStatus::MAINTENANCE) {
                        throw "Phòng đang trong quá trình bảo trì!!";
                    } else if (room->get_status() == RoomStatus::FULL) {
                        throw "Phòng đã đầy!!";
                    } else {
                        if (room->get_current_number() == room->get_capacity()) {
                            room->set_status(RoomStatus::FULL);
                            throw "Phòng đã đầy!!";
                        } else {
                            this->room_id = room_id;
                            room->set_current_number(room->get_current_number() + 1);
                            if (room->get_current_number() == room->get_capacity()) {
                                room->set_status(RoomStatus::FULL);
                            }
                            room_db->modify(room->get_id(), room.get());
                        }
                    }
                } else {
                    throw "Không tìm thấy phòng!!";
                }

                auto old_room = room_db->get_room(old_room_id);
                if (old_room) {
                    if (old_room->get_current_number() > 0)
                        old_room->set_current_number(old_room->get_current_number() - 1);
                    if (old_room->get_current_number() < old_room->get_capacity()) {
                        old_room->set_status(RoomStatus::AVAILABLE);
                    }
                    room_db->modify(old_room->get_id(), old_room.get());
                }
            } else {
                throw "Lỗi kết nối cơ sở dữ liệu!!";
            }
        } catch (const string& e) {
            throw e;
        }
    }
}

void Student::on_remove() {
    if (!removed_flag) {
        removed_flag = true;
    }
    if (removed_flag) {
        removed_flag = false;
        try {
            auto room_db = ModelProducer::get_instance(ModelType::ROOM);
            if (room_db) {
                auto room = room_db->get_room(room_id);
                if (room) {
                    if (room->get_status() == RoomStatus::FULL) {
                        room->set_status(RoomStatus::AVAILABLE);
                    }
                    
                    if (room->get_current_number() > 0) {
                        room->set_current_number(room->get_current_number() - 1);
                        room_db->modify(room->get_id(), room.get());
                    }
                }
            }
        } catch (const string& msg) {
            throw msg;
        }
    }
}


void Student::set_room_id(const string& room_id) {
    if (room_id == NO_ROOM_ID) {
        old_room_id = this->room_id;
        this->room_id = "";
        modified_flag = true;
        return;
    }
    
    // Check if the room_id is in right format.
    if (room_id.size() < 1 || room_id[0] < 'A' || room_id[0] > 'F') {
        this->room_id = "";
        return;
    }

    if (room_id.size() < 2 || room_id[1] < '0' || room_id[1] > '5') {
        this->room_id = "";
        return;
    }

    if (room_id.size() < 4) {
        this->room_id = "";
        return;
    }


    if (old_room_id == room_id) {
        return;
    }
    old_room_id = this->room_id;
    this->room_id = room_id;
    modified_flag = true;

}