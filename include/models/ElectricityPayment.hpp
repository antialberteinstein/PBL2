#include "Payment.hpp"
#pragma once

constexpr char electricity_payment_room_id[] = "room_id";

struct ElectricityPayment : Payment {
    int room_id;

    virtual string serialize() {
        return json{
            {payment_id, id},
            {payment_amount, amount},
            {payment_date_created, date_created},
            {payment_status, status},
            {payment_date_paid, date_paid},
            {electricity_payment_room_id, room_id},
        }.dump();
    }

    virtual void deserialize(string data) {
        auto j = json::parse(data);
        id = j[payment_id];
        amount = j[payment_amount];
        date_created = j[payment_date_created];
        status = j[payment_status];
        date_paid = j[payment_date_paid];
        room_id = j[electricity_payment_room_id];
    }
};