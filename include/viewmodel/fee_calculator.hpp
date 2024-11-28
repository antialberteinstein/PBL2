#pragma once

#include "models/Payment.hpp"
#include "models/Student.hpp"

#define PERIOD_DAYS 90
#define OVERDUE_DAYS 30

namespace FeeValue {
    // Phi phong nay tuong ung voi phong chi co 1 nguoi o
    //   nghia la neu co nhieu nguoi o thi so tien nay se duoc chia ra.
    const int ROOM_FEE = 600000;  // VND/ 1 month / 1 student / 1 room
    const int ELECTRICITY_FEE = 3000;  // VND/ 1 kWh
}

enum class FeeType {
    ROOM_FEE,
    ELECTRICITY_FEE,
};

class FeeCalculator {
    public:
        
        unique_ptr<Payment> get_payment(Student* student);

        void pay_by(Student* student);        

        static void init();

        static FeeCalculator* get_instance(FeeType type) {
            if (type == FeeType::ROOM_FEE) {
                if (room_fee_instance_ == nullptr) {
                    room_fee_instance_ = new FeeCalculator();
                }
                if (room_fee_instance_ != nullptr) {
                    room_fee_instance_->type_ = FeeType::ROOM_FEE;
                }
                return room_fee_instance_;
            } else if (type == FeeType::ELECTRICITY_FEE) {
                if (electricity_fee_instance_ == nullptr) {
                    electricity_fee_instance_ = new FeeCalculator();
                }
                if (electricity_fee_instance_ != nullptr) {
                    electricity_fee_instance_->type_ = FeeType::ELECTRICITY_FEE;
                }
                return electricity_fee_instance_;
            }
            return nullptr;
        }

        static void cleanup() {
            if (room_fee_instance_) {
                delete room_fee_instance_;
            }
            room_fee_instance_ = nullptr;
            if (electricity_fee_instance_) {
                delete electricity_fee_instance_;
            }
            electricity_fee_instance_ = nullptr;
        }

    private:
        static FeeCalculator* room_fee_instance_;
        static FeeCalculator* electricity_fee_instance_;

        FeeType type_;


        void create_payment(Student* student);

};