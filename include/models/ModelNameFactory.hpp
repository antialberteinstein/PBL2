#ifndef MODEL_NAME_FACTORY_HPP
#define MODEL_NAME_FACTORY_HPP

#include "models/ElectricityPayment.hpp"
#include "models/RoomFeePayment.hpp"
#include "models/Room.hpp"
#include "models/Student.hpp"
#include "models/Vehicle.hpp"


enum class ModelType {
    STUDENT,
    ROOM,
    VEHICLE,
    ROOM_FEE_PAYMENT,
    ELECTRICITY_PAYMENT,
};

class ModelNameFactory {
    public:
        static string get_name(ModelType type) {
            switch (type) {
                case ModelType::STUDENT:
                    return "student";
                case ModelType::ROOM:
                    return "room";
                case ModelType::VEHICLE:
                    return "vehicle";
                case ModelType::ROOM_FEE_PAYMENT:
                    return "room_fee_payment";
                case ModelType::ELECTRICITY_PAYMENT:
                    return "electricity_payment";
            }
            return "no_name";
        }
};

#endif // MODEL_NAME_FACTORY_HPP