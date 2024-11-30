#include "models/Vehicle.hpp"
#include "viewmodel/my_view_model.hpp"


Vector<Vehicle> Vehicle::get_vehicles(string student_id) {
    Vector<Vehicle> vehicles;
    try {
        auto vehicle_db = ModelProducer::get_instance(ModelType::VEHICLE);
        if (vehicle_db) {
            string _id = student_id;
            while (true) {
                auto vehicle = vehicle_db->get_vehicle(_id);
                if (vehicle == nullptr) {
                    break;
                }
                debug("Added " + vehicle->get_type());
                vehicles.push_back(*vehicle);
                _id += "@" + student_id;
            }
        }
    } catch (...) {
        debug("There is an error!");
    }

    return vehicles;
}

string Vehicle::hash_to_id() {
    if (creating_flag) {
        string new_id = student_id;
        try {
            auto vehicle_db = ModelProducer::get_instance(ModelType::VEHICLE);
            if (vehicle_db) {
                while (true) {
                    auto vehicle = vehicle_db->get_vehicle(new_id);
                    if (vehicle == nullptr) {
                        break;
                    }
                    debug("Loaded " + new_id);
                    new_id = "@" + student_id;
                }
            }
        } catch (...) {
            // Do nothing.
        }
        return new_id;
    } else {
        return id;
    }
}