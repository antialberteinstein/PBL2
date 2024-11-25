#include "models/Room.hpp"

#include "viewmodel/my_view_model.hpp"
#include "models/Student.hpp"


// Define a trigger
void Room::on_modify() {
    //  Called when the room is notified that
    //      it is not available(maintenance)
    if (modified_flag) {
        debug("Room modified");
        modified_flag = false;

        try {
            auto student_db = ModelProducer::get_instance(ModelType::STUDENT);
            auto students = Student::get_students_living_in(id);
            for (int i = 0; i < students.size(); i++) {
                auto student_ptr = student_db->get_student(students[i].get_id());
                if (student_ptr) {
                    student_ptr->set_room_id("");
                    student_db->modify(student_ptr->get_id(), student_ptr.get());
                    debug("Student " + student_ptr->get_id() + "'s room is " + student_ptr->get_room_id());
                }
            }
        } catch (const string& msg) {
            throw msg;
        }
        
    }
}

void Room::set_status(const string& status) {
    this->status = status;
    if (status == RoomStatus::MAINTENANCE) {
        current_number = 0;
        modified_flag = true;
    }  
}