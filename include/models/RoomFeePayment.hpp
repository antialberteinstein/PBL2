#include "Payment.hpp"
#include "Student.hpp"
#pragma once

constexpr char room_fee_payment_number_of_months[] = "number_of_months";
constexpr char room_fee_payment_student_id[] = "student_id";

class ModelProducer;

class RoomFeePayment : public Payment {
private:
    int number_of_months;
    string student_id;

public:
    virtual string serialize() {
        return json{
            {payment_id, id},
            {payment_amount, amount},
            {payment_date_created, date_created},
            {payment_status, status},
            {payment_date_paid, date_paid},
            {room_fee_payment_number_of_months, number_of_months},
            {room_fee_payment_student_id, student_id},
        }.dump();
    }

    virtual void deserialize(string data) {
        auto j = json::parse(data);
        id = j[payment_id];
        amount = j[payment_amount];
        date_created = j[payment_date_created];
        status = j[payment_status];
        date_paid = j[payment_date_paid];
        number_of_months = j[room_fee_payment_number_of_months];
        student_id = j[room_fee_payment_student_id];
    }

    int get_number_of_months() {
        return number_of_months;
    }

    string get_student_id() {
        return student_id;
    }
    
    unique_ptr<Student> get_student();

    void set_number_of_months(int number_of_months) {
        this->number_of_months = number_of_months;
    }

    void set_student_id(string student_id) {
        this->student_id = student_id;
    }

    virtual string hash_to_id() {
        if (overriden_flag) {
            return student_id;
        } else {
            return student_id + "_" + date_created;
        }
    }

};
