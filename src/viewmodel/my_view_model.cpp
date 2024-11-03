#include "viewmodel/my_view_model.hpp"

void debug(const string& s) {
    std::ofstream out("temp/debug.txt", std::ios::app);
    out << s << std::endl;
    out.close();
}


unique_ptr<ModelProducer> ModelProducer::student_instance_ = nullptr;
unique_ptr<ModelProducer> ModelProducer::room_instance_ = nullptr;
unique_ptr<ModelProducer> ModelProducer::vehicle_instance_ = nullptr;
unique_ptr<ModelProducer> ModelProducer::room_fee_payment_instance_ = nullptr;
unique_ptr<ModelProducer> ModelProducer::electricity_payment_instance_ = nullptr;

leveldb::Options ModelProducer::options_ = leveldb::Options();

ModelProducer* ModelProducer::get_instance(ModelType type) {
    if (!options_.create_if_missing) {
        options_.create_if_missing = true;
    }

    switch (type) {
        case ModelType::STUDENT: {
            if (student_instance_ == nullptr) {
                student_instance_ = make_unique<ModelProducer>();
                leveldb::Status status =
                    leveldb::DB::Open(options_,
                    _DB_PATH + ModelNameFactory::get_name(ModelType::STUDENT),
                    &student_instance_->db_
                );
                if (!status.ok()) {
                    throw runtime_error("Error opening database: " + status.ToString());
                }
            }
            student_instance_->type_ = ModelType::STUDENT;
            return student_instance_.get();
        }
        case ModelType::ROOM: {
            if (room_instance_ == nullptr) {
                room_instance_ = make_unique<ModelProducer>();
                leveldb::Status status =
                    leveldb::DB::Open(options_,
                    _DB_PATH + ModelNameFactory::get_name(ModelType::ROOM),
                    &room_instance_->db_
                );
                if (!status.ok()) {
                    throw runtime_error("Error opening database: " + status.ToString());
                }
            }
            room_instance_->type_ = ModelType::ROOM;
            return room_instance_.get();
        }
        case ModelType::VEHICLE: {
            if (vehicle_instance_ == nullptr) {
                vehicle_instance_ = make_unique<ModelProducer>();
                leveldb::Status status =
                    leveldb::DB::Open(options_,
                    _DB_PATH + ModelNameFactory::get_name(ModelType::VEHICLE),
                    &vehicle_instance_->db_
                );
                if (!status.ok()) {
                    throw runtime_error("Error opening database: " + status.ToString());
                }
            }
            vehicle_instance_->type_ = ModelType::VEHICLE;
            return vehicle_instance_.get();
        }
        case ModelType::ROOM_FEE_PAYMENT: {
            if (room_fee_payment_instance_ == nullptr) {
                room_fee_payment_instance_ = make_unique<ModelProducer>();
                leveldb::Status status =
                    leveldb::DB::Open(options_,
                    _DB_PATH + ModelNameFactory::get_name(ModelType::ROOM_FEE_PAYMENT),
                    &room_fee_payment_instance_->db_
                );
                if (!status.ok()) {
                    throw runtime_error("Error opening database: " + status.ToString());
                }
            }
            room_fee_payment_instance_->type_ = ModelType::ROOM_FEE_PAYMENT;
            return room_fee_payment_instance_.get();
        }
        case ModelType::ELECTRICITY_PAYMENT: {
            if (electricity_payment_instance_ == nullptr) {
                electricity_payment_instance_ = make_unique<ModelProducer>();
                leveldb::Status status =
                    leveldb::DB::Open(options_,
                    _DB_PATH + ModelNameFactory::get_name(ModelType::ELECTRICITY_PAYMENT),
                    &electricity_payment_instance_->db_
                );
                if (!status.ok()) {
                    throw runtime_error("Error opening database: " + status.ToString());
                }
            }
            electricity_payment_instance_->type_ = ModelType::ELECTRICITY_PAYMENT;
            return electricity_payment_instance_.get();
        }
    }
    return nullptr;
}