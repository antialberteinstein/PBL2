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
};

namespace PaymentStatus {
    const string UNPAID = "Unpaid";
    const string PAID = "Paid";
    const string OVERDUE = "Overdue";
}
