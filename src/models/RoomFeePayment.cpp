#include "models/RoomFeePayment.hpp"
#include "viewmodel/my_view_model.hpp"
#include "objects/Date.hpp"
#include "viewmodel/fee_calculator.hpp"

unique_ptr<Student> RoomFeePayment::get_student() {
    try {
        auto student_db = ModelProducer::get_instance(ModelType::STUDENT);
        if (student_db)
            return student_db->get_student(student_id);
        else
            return nullptr;
    } catch (const string& e) {
        return nullptr;
    }
}

// bool RoomFeePayment::onPrepareToAdd() {
//     if (prepare_added_flag) {
//         prepare_added_flag = false;
//         auto rf_db = ModelProducer::get_instance(ModelType::ROOM_FEE_PAYMENT);
//         if (rf_db == nullptr) {
//             return false;
//         }

//         this->flag_overriden();
//         auto old_rf = rf_db->get_room_fee_payment(this->student_id);
//         if (old_rf != nullptr) {
//             old_rf->flag_not_overriden();
//             if (old_rf->get_status() == PaymentStatus::PAID) {
//                 Date date_paid = DateConverter::from_grenadian_string(old_rf->get_date_paid());
//                 if (date_paid + PERIOD_DAYS < Date::today()) {
//                     return false;
//                 } else {
//                     rf_db->add(old_rf.get());
//                     rf_db->add(this);
//                 }
//             } else if (old_rf->get_status() == PaymentStatus::UNPAID) {
//                 Date date_created = DateConverter::from_grenadian_string(old_rf->get_date_created());
//                 if (date_created + OVERDUE_DAYS < Date::today()) {
//                     old_rf->set_status(PaymentStatus::OVERDUE);
//                     old_rf->flag_overriden();
//                     rf_db->modify(old_rf->get_id(), old_rf.get());
//                 }
//             } else if (old_rf->get_status() == PaymentStatus::OVERDUE) {
//                 auto student_db = ModelProducer::get_instance(ModelType::STUDENT);
//                 if (student_db != nullptr) {
//                     auto student = student_db->get_student(old_rf->get_student_id());
//                     if (student != nullptr) {
//                         student->set_room_id("");
//                         student_db->modify(student->get_id(), student.get());
//                     }
//                 }
//             }
//         }
//     }
// }
