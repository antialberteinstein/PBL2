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
                vehicles.push_back(*vehicle);
                _id += "@" + student_id;
            }
        }
    } catch (...) {
        // Do nah thing.
    }

    return vehicles;
}