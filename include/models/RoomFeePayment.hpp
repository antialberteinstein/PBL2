#include "Payment.hpp"

#pragma once

constexpr char room_fee_payment_number_of_months[] = "number_of_months";
constexpr char room_fee_payment_student_id[] = "student_id";

struct RoomFeePayment : Payment {
    int number_of_months;
    int student_id;

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
};