#include <iostream>
#include "models/Model.hpp"

using namespace std;

#pragma once

constexpr char payment_id[] = "payment_id";
constexpr char payment_amount[] = "amount";
constexpr char payment_date_created[] = "date_created";
constexpr char payment_status[] = "status";
constexpr char payment_date_paid[] = "date_paid";


struct Payment : Model {
    long long amount;
    string date_created;
    string status;
    string date_paid;

    virtual string serialize() = 0;
    virtual void deserialize(string data) = 0;
};

namespace PaymentStatus {
    const string UNPAID = "Unpaid";
    const string PAID = "Paid";
    const string OVERDUE = "Overdue";
}