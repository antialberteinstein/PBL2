#include "viewmodel/fee_calculator.hpp"

#include "models/RoomFeePayment.hpp"
#include "models/ElectricityPayment.hpp"
#include "viewmodel/my_view_model.hpp"
#include "objects/Date.hpp"
#include "objects/Vector.hpp"

#include <iostream>

using namespace std;

FeeCalculator* FeeCalculator::room_fee_instance_ = nullptr;
FeeCalculator* FeeCalculator::electricity_fee_instance_ = nullptr;

unique_ptr<Payment> FeeCalculator::get_payment(Student* student) {
    if (type_ != FeeType::ROOM_FEE) {
        return nullptr;
    }

    
    try {
        create_payment(student);
        auto room_fee_db = ModelProducer::get_instance(ModelType::ROOM_FEE_PAYMENT);
        if (room_fee_db) {
            auto room_fee_payment = room_fee_db->get_room_fee_payment(student->get_id());
            if (room_fee_payment != nullptr) {
                return move(room_fee_payment);
            }

        }
    } catch (const string& msg) {
        throw msg;
    }

    return nullptr;
}

void FeeCalculator::create_payment(Student* student) {
    auto _today = Date::today();
    auto room_fee_payment_db = ModelProducer::get_instance(ModelType::ROOM_FEE_PAYMENT);
    if (room_fee_payment_db == nullptr) {
        return;
    }

    auto room_fee_payment = make_unique<RoomFeePayment>();

    room_fee_payment->set_date_created(_today.to_string());
    auto room = student->get_room_ref();
    if (room == nullptr) {
        return;
    }
    int amount = FeeValue::ROOM_FEE / room->get_capacity() * 3;  // 3 months.  
    room_fee_payment->set_amount(amount);
    room_fee_payment->set_number_of_months(3);
    room_fee_payment->set_status(PaymentStatus::UNPAID);
    room_fee_payment->set_student_id(student->get_id());
    room_fee_payment->flag_overriden();

    auto old_room_fee_payment = room_fee_payment_db
        ->get_room_fee_payment(room_fee_payment->hash_to_id());

    if (old_room_fee_payment == nullptr) {
        room_fee_payment_db->add(room_fee_payment.get());
        debug("Created a new payment cause of no old payment");
    } else {
        if (old_room_fee_payment->get_status() == PaymentStatus::PAID) {
            Date date_paid = DateConverter::from_grenadian_string(old_room_fee_payment->get_date_paid());
            if (date_paid + PERIOD_DAYS < _today) {
                return;
            } else {
                old_room_fee_payment->flag_not_overriden();
                room_fee_payment_db->add(old_room_fee_payment.get());
                room_fee_payment_db->add(room_fee_payment.get());
            }
            debug("Old payment is paid at " + old_room_fee_payment->get_date_paid());
        }
        if (old_room_fee_payment->get_status() == PaymentStatus::UNPAID) {
            Date date_created = DateConverter::from_grenadian_string(old_room_fee_payment->get_date_created());
            if (date_created + OVERDUE_DAYS < _today) {
                old_room_fee_payment->set_status(PaymentStatus::OVERDUE);
                old_room_fee_payment->flag_overriden();
                room_fee_payment_db->modify(old_room_fee_payment->get_id(), old_room_fee_payment.get());
                debug("Old payment is unpaid and overdue");
            }
        }

        if (old_room_fee_payment->get_status() == PaymentStatus::OVERDUE) {
            auto student_db = ModelProducer::get_instance(ModelType::STUDENT);
            if (student_db != nullptr) {
                auto student = student_db->get_student(old_room_fee_payment->get_student_id());
                if (student != nullptr) {

                    // Force student to leave the room.
                    student->set_room_id("");
                    student_db->modify(student->get_id(), student.get());
                }
            }
        }
    }


}

void FeeCalculator::pay_by(Student* student) {
    auto room_fee_db = ModelProducer::get_instance(ModelType::ROOM_FEE_PAYMENT);
    if (room_fee_db == nullptr) {
        throw "Không thể thanh toán";
    }

    auto room_fee_payment = room_fee_db->get_room_fee_payment(student->get_id());
    if (room_fee_payment == nullptr) {
        throw "Không thể thanh toán";
    }

    room_fee_payment->set_status(PaymentStatus::PAID);
    room_fee_payment->set_date_paid(Date::today().to_string());
    room_fee_db->modify(room_fee_payment->get_id(), room_fee_payment.get());
}


void FeeCalculator::init() {
    try {
        auto student_db = ModelProducer::get_instance(ModelType::STUDENT);
        auto rf_cal = FeeCalculator::get_instance(FeeType::ROOM_FEE);
        if (student_db != nullptr && rf_cal != nullptr) {
            Vector<string> keys = student_db->get_all_keys();
            for (int i = 0; i < keys.size(); ++i) {
                auto student = student_db->get_student(keys[i]);
                debug("Created student");
                if (student != nullptr) {
                    rf_cal->create_payment(student.get());
                    debug("Created payment");
                }
            }
        }
    } catch (const string& msg) {
        debug(msg);
    }
}
