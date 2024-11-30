#include <iostream>
#include "models/Model.hpp"
#include "objects/Vector.hpp"

using namespace std;

#pragma once

constexpr char vehicle_id[] = "vehicle_id";
constexpr char vehicle_type[] = "type";
constexpr char vehicle_student_id[] = "student_id";

class Vehicle : public Model {
    private:
    string type;
    string student_id;
    bool creating_flag;

    public:

    void set_type(const string& type) {
        this->type = type;
    }

    string get_type() {
        return type;
    }

    void set_student_id(const string& student_id) {
        this->student_id = student_id;
    }

    string get_student_id() {
        return student_id;
    }

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

    virtual string hash_to_id();

    void flag_creating() {
        creating_flag = true;
    }

    void flag_not_creating() {
        creating_flag = false;
    }

    Vehicle() {
        id = "";
        creating_flag = false;
    }

    static Vector<Vehicle> get_vehicles(string student_id);


};

namespace VehicleType {
    const string MOTORBIKE = "Xe máy";
    const string CAR = "Ô tô";
    const string BICYCLE = "Xe đạp";
}

namespace VehicleFee {
    const int MOTORBIKE = 50000;  // VND/ 1 month / 1 vehicle
    const int CAR = 100000;  // VND/ 1 month / 1 vehicle
    const int BICYCLE = 10000;  // VND/ 1 month / 1 vehicle
}

