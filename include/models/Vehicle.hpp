#include <iostream>
#include "models/Model.hpp"

using namespace std;

#pragma once

constexpr char vehicle_id[] = "vehicle_id";
constexpr char vehicle_type[] = "type";
constexpr char vehicle_student_id[] = "student_id";

struct Vehicle : Model {
    string type;
    string student_id;

    virtual string serialize() {
        return json{
            {vehicle_id, id},
            {vehicle_type, type},
            {vehicle_student_id, student_id},
        }.dump();
    }

    virtual void deserialize(string data) {
        auto j = json::parse(data);
        id = j[vehicle_id];
        type = j[vehicle_type];
        student_id = j[vehicle_student_id];
    }

    virtual string hash_to_id() {
        return type + "_" + student_id;
    }
};

namespace VehicleType {
    const string MOTORBIKE = "Motorbike";
    const string CAR = "Car";
    const string BICYCLE = "Bicycle";
}
