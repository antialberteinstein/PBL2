#pragma once

#include "models/Payment.hpp"
#include "models/Student.hpp"

#define PERIOD_DAYS 90
#define OVERDUE_DAYS 30
#define ENERGY_OVERDUE_DAYS 10

enum class FeeType {
    ROOM_FEE,
    ELECTRICITY_FEE,
};

class FeeCalculator {
    public:
        
        unique_ptr<Payment> get_payment(Student* student);
        unique_ptr<Payment> get_payment(Room* room);

        void pay_by(Student* student);
        void pay_by(Room* room);      

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

        json energy_consumption_data;
        string energy_consumption_date;

        void create_payment(Student* student);
        void create_payment(Room* room);

        
        bool energy_consumption_load_data();

        bool energy_consumption_get_from_server();

};