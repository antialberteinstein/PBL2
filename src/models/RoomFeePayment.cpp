#include "models/RoomFeePayment.hpp"
#include "viewmodel/my_view_model.hpp"

unique_ptr<Student> RoomFeePayment::get_student() {
    try {
        auto student_db = ModelProducer::get_instance(ModelType::STUDENT);
        return student_db->get_student(student_id);
    } catch (const string& e) {
        return nullptr;
    }
}
