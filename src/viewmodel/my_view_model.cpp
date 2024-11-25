#include "viewmodel/my_view_model.hpp"
#include <csignal>

void debug(const string& s) {
    std::ofstream out("temp/debug.txt", std::ios::app);
    out << s << std::endl;
    out.close();
}


ModelProducer* ModelProducer::student_instance_ = nullptr;
ModelProducer* ModelProducer::room_instance_ = nullptr;
ModelProducer* ModelProducer::vehicle_instance_ = nullptr;
ModelProducer* ModelProducer::room_fee_payment_instance_ = nullptr;
ModelProducer* ModelProducer::electricity_payment_instance_ = nullptr;

leveldb::Options ModelProducer::options_ = leveldb::Options();

ModelProducer* ModelProducer::get_instance(ModelType type) {
    if (!options_.create_if_missing) {
        options_.create_if_missing = true;
    }

    switch (type) {
        case ModelType::STUDENT: {
            if (student_instance_ == nullptr) {
                student_instance_ = new ModelProducer();
                leveldb::Status status =
                    leveldb::DB::Open(options_,
                    _DB_PATH + ModelNameFactory::get_name(ModelType::STUDENT),
                    &student_instance_->db_
                );
                if (!status.ok()) {
                    throw "Error opening database: " + status.ToString();
                }
            }
            student_instance_->type_ = ModelType::STUDENT;
            // return student_instance_.get();
            return student_instance_;
        }
        case ModelType::ROOM: {
            if (room_instance_ == nullptr) {
                room_instance_ = new ModelProducer();
                leveldb::Status status =
                    leveldb::DB::Open(options_,
                    _DB_PATH + ModelNameFactory::get_name(ModelType::ROOM),
                    &room_instance_->db_
                );
                if (!status.ok()) {
                    throw "Error opening database: " + status.ToString();
                }
            }
            room_instance_->type_ = ModelType::ROOM;
            // return room_instance_.get();
            return room_instance_;
        }
        case ModelType::VEHICLE: {
            if (vehicle_instance_ == nullptr) {
                vehicle_instance_ = new ModelProducer();
                leveldb::Status status =
                    leveldb::DB::Open(options_,
                    _DB_PATH + ModelNameFactory::get_name(ModelType::VEHICLE),
                    &vehicle_instance_->db_
                );
                if (!status.ok()) {
                    throw "Error opening database: " + status.ToString();
                }
            }
            vehicle_instance_->type_ = ModelType::VEHICLE;
            // return vehicle_instance_.get();
            return vehicle_instance_;
        }
        case ModelType::ROOM_FEE_PAYMENT: {
            if (room_fee_payment_instance_ == nullptr) {
                room_fee_payment_instance_ = new ModelProducer();
                leveldb::Status status =
                    leveldb::DB::Open(options_,
                    _DB_PATH + ModelNameFactory::get_name(ModelType::ROOM_FEE_PAYMENT),
                    &room_fee_payment_instance_->db_
                );
                if (!status.ok()) {
                    throw "Error opening database: " + status.ToString();
                }
            }
            room_fee_payment_instance_->type_ = ModelType::ROOM_FEE_PAYMENT;
            // return room_fee_payment_instance_.get();
            return room_fee_payment_instance_;
        }
        case ModelType::ELECTRICITY_PAYMENT: {
            if (electricity_payment_instance_ == nullptr) {
                electricity_payment_instance_ = new ModelProducer();
                leveldb::Status status =
                    leveldb::DB::Open(options_,
                    _DB_PATH + ModelNameFactory::get_name(ModelType::ELECTRICITY_PAYMENT),
                    &electricity_payment_instance_->db_
                );
                if (!status.ok()) {
                    throw "Error opening database: " + status.ToString();
                }
            }
            electricity_payment_instance_->type_ = ModelType::ELECTRICITY_PAYMENT;
            // return electricity_payment_instance_.get();
            return electricity_payment_instance_;
        }
    }
    return nullptr;
}

void signal_handler(int signum) {
    debug("Interrupt signal (" + to_string(signum) + ") received.");

    ModelProducer::cleanup();

    exit(signum);
}

void ModelProducer::init() {
    try {
        atexit(ModelProducer::cleanup);

        signal(SIGINT, signal_handler);
        signal(SIGTERM, signal_handler);
        signal(SIGABRT, signal_handler);
        signal(SIGSEGV, signal_handler);
        signal(SIGILL, signal_handler);
        signal(SIGFPE, signal_handler);  // Linux only, floating point exception
        signal(SIGBUS, signal_handler);  // Linux only, bus error
    } catch (const std::exception& e) {
        debug(e.what());
    } catch (...) {
        debug("Unknown error");
    }
}

void ModelProducer::cleanup() {
    if (student_instance_ != nullptr) {
        delete student_instance_;
    }

    if (room_instance_ != nullptr) {
        delete room_instance_;
    }

    if (vehicle_instance_ != nullptr) {
        delete vehicle_instance_;
    }

    if (room_fee_payment_instance_ != nullptr) {
        delete room_fee_payment_instance_;
    }

    if (electricity_payment_instance_ != nullptr) {
        delete electricity_payment_instance_;
    }

    student_instance_ = nullptr;
    room_instance_ = nullptr;
    vehicle_instance_ = nullptr;
    room_fee_payment_instance_ = nullptr;
    electricity_payment_instance_ = nullptr;
}
