#include "Payment.hpp"
#include "Room.hpp"
#pragma once

constexpr char electricity_payment_room_id[] = "room_id";

class ModelProducer;

class ElectricityPayment : public Payment {
private:
    string room_id;
public:
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

    string get_room_id() {
        return room_id;
    }

    unique_ptr<Room> get_room();

    void set_room_id(string room_id) {
        this->room_id = room_id;
    }

};
