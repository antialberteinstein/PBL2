#include "viewmodel/fee_calculator.hpp"

#include "models/RoomFeePayment.hpp"
#include "models/ElectricityPayment.hpp"
#include "viewmodel/my_view_model.hpp"
#include "objects/Date.hpp"
#include "objects/Vector.hpp"
#include <filesystem>
#include "models/Vehicle.hpp"

#include <iostream>

using namespace std;


namespace Retriever {
    const string DATA_PATH = "res/data/current_month_energy_consumption.json";
    const char* EXECUTE = "./res/bin/retrieve_data";
    const string DATE_PATH = "res/data/date.txt";
    const string GOOD_FILE = "res/data/good";
    const string LOCK_FILE = "res/data/lock";
    const int retrieving_date = 25;
}

namespace FeeValue {
    // Phi phong nay tuong ung voi phong chi co 1 nguoi o
    //   nghia la neu co nhieu nguoi o thi so tien nay se duoc chia ra.
    const int ROOM_FEE = 600000;  // VND/ 1 month / 1 student / 1 room
    const int ELECTRICITY_FEE = 3000;  // VND/ 1 kWh
}


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
    int vehicle_fee = 0;
    Vector<Vehicle> vehicles = Vehicle::get_vehicles(student->get_id());
    for (int i = 0; i < vehicles.size(); ++i) {
        if (vehicles[i].get_type() == VehicleType::MOTORBIKE) {
            vehicle_fee += VehicleFee::MOTORBIKE;
        } else if (vehicles[i].get_type() == VehicleType::BICYCLE) {
            vehicle_fee += VehicleFee::BICYCLE;
        } else if (vehicles[i].get_type() == VehicleType::CAR) {
            vehicle_fee += VehicleFee::CAR;
        }
    }

    int amount = (FeeValue::ROOM_FEE / room->get_capacity() + vehicle_fee) * 3;  // 3 months.  
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

    if (room_fee_payment->get_status() == PaymentStatus::PAID) {
        throw "Sinh viên này đã thanh toán tiền phòng";
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



    try {
        auto ef_cal = FeeCalculator::get_instance(FeeType::ELECTRICITY_FEE);
        if (ef_cal != nullptr) {
            if (ef_cal->energy_consumption_get_from_server()) {
                if (ef_cal->energy_consumption_load_data()) {
                    auto room_db = ModelProducer::get_instance(ModelType::ROOM);
                    if (room_db) {
                        Vector<string> keys = room_db->get_all_keys();
                        for (int i = 0; i < keys.size(); ++i) {
                            auto room = room_db->get_room(keys[i]);
                            if (room) {
                                ef_cal->create_payment(room.get());
                            }
                        }
                    }
                }
            }
        }

    } catch (const string& msg) {
        debug(msg);
    }
}

bool FeeCalculator::energy_consumption_load_data() {
    ifstream date_file(Retriever::DATE_PATH);

    if (!date_file.is_open()) {
        return false;
    }

    date_file >> energy_consumption_date;

    date_file.close();

    if (energy_consumption_date == "") {
        return false;
    }

    try {
        Date today = Date::today();
        if (today < DateConverter::from_grenadian_string(energy_consumption_date)) {
            return false;
        }
    } catch (...) {
        return false;
    }

    ifstream data_file(Retriever::DATA_PATH);

    if (!data_file.is_open()) {
        return false;
    }

    try {
        data_file >> energy_consumption_data;
    } catch (...) {
        return false;
    }

    return true;
}

bool FeeCalculator::energy_consumption_get_from_server() {
    // Run executable file for retrieve data to res/data
    if (system(Retriever::EXECUTE)) {
        return false;
    }

    return true;
}

void FeeCalculator::create_payment(Room* room) {
    auto ef_db = ModelProducer::get_instance(ModelType::ROOM_FEE_PAYMENT);

    if (ef_db == nullptr) {
        return;
    }

    auto ef = ef_db->get_electricity_payment(room->get_id_string());
    long old_amount = 0;
    if (ef != nullptr) {
        ef->flag_not_overriden();
        if (ef->get_status() == PaymentStatus::UNPAID) {
            // Chuyen tien dien ky truoc sang ky sau.
            old_amount = ef->get_amount();
            ef->set_status(PaymentStatus::OVERDUE);
        }

        ef_db->add(ef.get());
    }
    ef = make_unique<ElectricityPayment>();
    ef->set_date_created(energy_consumption_date);
    
    float energy_consumption = 0;
    try {
        energy_consumption = energy_consumption_data[room->get_id_string()];
    } catch (...) {
        energy_consumption = 0;
    }
    long new_amount = old_amount + long(energy_consumption * FeeValue::ELECTRICITY_FEE);
    ef->set_amount(new_amount);
    
    ef->set_status(PaymentStatus::UNPAID);
    ef->set_room_id(room->get_id_string());
    ef->flag_overriden();
    ef_db->modify(ef->get_id(), ef.get());
}

unique_ptr<Payment> FeeCalculator::get_payment(Room* room) {
    try {
        auto ef_db = ModelProducer::get_instance(ModelType::ELECTRICITY_PAYMENT);
        if (ef_db) {
            auto ef = ef_db->get_electricity_payment(room->get_id_string());
            if (ef != nullptr) {
                return move(ef);
            }
        }
    } catch (const string& msg) {
        return nullptr;
    }
    return nullptr;
}

void FeeCalculator::pay_by(Room* room) {
    auto ef_db = ModelProducer::get_instance(ModelType::ELECTRICITY_PAYMENT);
    if (ef_db == nullptr) {
        throw "Không thể thanh toán";
    }
    auto ef = ef_db->get_electricity_payment(room->get_id_string());
    if (ef == nullptr) {
        throw "Không thể thanh toán";
    }

    if (ef->get_status() == PaymentStatus::UNPAID) {
        throw "Phòng này đã thanh toán tất cả tiền điện";
    }

    ef->set_status(PaymentStatus::PAID);
            ef->set_date_paid(Date::today().to_string());
            ef_db->modify(ef->get_id(), ef.get());
}