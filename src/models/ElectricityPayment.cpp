#include "models/ElectricityPayment.hpp"
#include "viewmodel/my_view_model.hpp"

unique_ptr<Room> ElectricityPayment::get_room() {
    try {
        auto room_db = ModelProducer::get_instance(ModelType::ROOM);
        return room_db->get_room(room_id);
    } catch (const string& e) {
        return nullptr;
    }
}
