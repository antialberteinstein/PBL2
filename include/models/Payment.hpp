#include <iostream>
#include "models/Model.hpp"

using namespace std;

#pragma once

constexpr char payment_id[] = "payment_id";
constexpr char payment_amount[] = "amount";
constexpr char payment_date_created[] = "date_created";
constexpr char payment_status[] = "status";
constexpr char payment_date_paid[] = "date_paid";


class Payment : public Model {
protected:
    long long amount;
    string date_created;
    string status;
    string date_paid;

public:
    virtual string serialize() = 0;
    virtual void deserialize(string data) = 0;
    virtual string hash_to_id() {
        return date_created + "_" + to_string(amount);
    };

    long long get_amount() {
        return amount;
    }

    string get_date_created() {
        return date_created;
    }

    string get_status() {
        return status;
    }

    string get_date_paid() {
        return date_paid;
    }

    void set_amount(long long amount) {
        this->amount = amount;
    }

    void set_date_created(string date_created) {
        this->date_created = date_created;
    }

    void set_status(string status) {
        this->status = status;
    }

    void set_date_paid(string date_paid) {
        this->date_paid = date_paid;
    }

    Payment() {
        overriden_flag = true;
    }

    void flag_overriden() {
        overriden_flag = true;
    }

    void flag_not_overriden() {
        overriden_flag = false;
    }

protected:

    // This flag is turned off for old payments that will be
    //         override by a new one, but still kept in the database.
    //         so, with this flag turned off, the system will not
    //         override the old payment.
    // This flag is turned on for a new payment, this will decide the payment
    //          having a specific id that can be easily found in the database.
    bool overriden_flag;
};

namespace PaymentStatus {
    const string UNPAID = "Chưa thanh toán";
    const string PAID = "Đã thanh toán";
    const string OVERDUE = "Quá hạn";
}
