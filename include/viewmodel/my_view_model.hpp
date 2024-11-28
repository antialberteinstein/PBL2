#ifndef MY_VIEW_MODEL_HPP
#define MY_VIEW_MODEL_HPP

#include "models/ModelNameFactory.hpp"
#include "objects/List.hpp"
#include "leveldb/db.h"
#include "objects/Queue.hpp"
#include "objects/Vector.hpp"
#include "fstream"

#define _DB_PATH "res/db/"
#define SIZE_KEY "size"

void debug(const string& s);

class ModelProducer {
    public:

        ModelProducer() {
            size_ = 0;
        }

        static ModelProducer* get_instance(ModelType type);

        virtual ~ModelProducer() {
            delete db_;
        }

        Vector<string> get_all_keys() {
            Vector<string> keys;
            leveldb::Iterator* it = db_->NewIterator(leveldb::ReadOptions());
            for (it->SeekToFirst(); it->Valid(); it->Next()) {
                keys.push_back(it->key().ToString());
            }
            delete it;
            return keys;
        }

        void add(Model* model) {
            try {
                if (!model->onPrepareToAdd()) {
                    throw "Cannot prepare to add!!";
                }
            } catch (const char* msg) {
                throw msg;
            }


            string _id = model->generate_id();
            if (_id == "") {
                return;
            }

            if (type_ == ModelType::STUDENT && get_student(_id) != nullptr) {
                throw "Sinh vien đã tồn tại";
            } else if (type_ == ModelType::ROOM && get_room(_id) != nullptr) {
                return;
            } else if (type_ == ModelType::VEHICLE && get_vehicle(_id) != nullptr) {
                return;
            } else if (type_ == ModelType::ROOM_FEE_PAYMENT && get_room_fee_payment(_id) != nullptr) {
                return;
            } else if (type_ == ModelType::ELECTRICITY_PAYMENT && get_electricity_payment(_id) != nullptr) {
                return;
            }

            leveldb::Status status = db_->Put(
                leveldb::WriteOptions(),
                _id,
                model->serialize()
            );
            
            if (!status.ok()) {
                throw "Error adding to database: " + status.ToString();
            } 
            size_++;
            dump_size();

            // Apply triggers
            try {
                model->on_add();
            } catch (const char* msg) {
                remove(model);
                throw msg;
            }
        }

        void remove(Model* model) {
            if (model == nullptr) {
                return;
            }
            string key = model->get_id();
            leveldb::Status status = db_->Delete(leveldb::WriteOptions(), key);
            if (!status.ok()) {
                throw "Error removing from database: " + status.ToString();
            }

            // Apply triggers
            try {
                model->on_remove();
            } catch (const char* msg) {
                add(model);
                throw msg;
            }

        }

        unique_ptr<Student> get_student(string key) {
            if (key == SIZE_KEY) {
                return nullptr;
            }
            string data;
            leveldb::Status status = db_->Get(leveldb::ReadOptions(), key, &data);
            if (!status.ok()) {
                return nullptr;
            }
            if (type_ == ModelType::STUDENT) {
                auto student = make_unique<Student>();
                student->deserialize(data);
                return move(student);
            } else {
                return nullptr;
            }
        }

        unique_ptr<Room> get_room(string key) {
            if (key == SIZE_KEY) {
                return nullptr;
            }
            string data;
            leveldb::Status status = db_->Get(leveldb::ReadOptions(), key, &data);
            if (!status.ok()) {
                return nullptr;
            }
            if (type_ == ModelType::ROOM) {
                auto room = make_unique<Room>();
                room->deserialize(data);
                return move(room);
            } else {
                return nullptr;
            }
        }

        unique_ptr<Vehicle> get_vehicle(string key) {
            if (key == SIZE_KEY) {
                return nullptr;
            }
            string data;
            leveldb::Status status = db_->Get(leveldb::ReadOptions(), key, &data);
            if (!status.ok()) {
                return nullptr;
            }
            if (type_ == ModelType::VEHICLE) {
                auto vehicle = make_unique<Vehicle>();
                vehicle->deserialize(data);
                return move(vehicle);
            } else {
                return nullptr;
            }
        }

        unique_ptr<RoomFeePayment> get_room_fee_payment(string key) {
            if (key == SIZE_KEY) {
                return nullptr;
            }
            string data;
            leveldb::Status status = db_->Get(leveldb::ReadOptions(), key, &data);
            if (!status.ok()) {
                return nullptr;
            }
            if (type_ == ModelType::ROOM_FEE_PAYMENT) {
                auto room_fee_payment = make_unique<RoomFeePayment>();
                room_fee_payment->deserialize(data);
                return move(room_fee_payment);
            } else {
                return nullptr;
            }
        }

        unique_ptr<ElectricityPayment> get_electricity_payment(string key) {
            if (key == SIZE_KEY) {
                return nullptr;
            }
            string data;
            leveldb::Status status = db_->Get(leveldb::ReadOptions(), key, &data);
            if (!status.ok()) {
                return nullptr;
            }
            if (type_ == ModelType::ELECTRICITY_PAYMENT) {
                auto electricity_payment = make_unique<ElectricityPayment>();
                electricity_payment->deserialize(data);
                return move(electricity_payment);
            } else {
                return nullptr;
            }
        }

        int size() {
            if (size_ == 0) {
                retrieve_size();
            }
            return size_;
        }

        void modify(string key, Model* model) {
            leveldb::Status status = db_->Put(leveldb::WriteOptions(), key, model->serialize());
            if (!status.ok()) {
                throw "Error modifying database: " + status.ToString();
            }

            // Apply triggers
            try {
                model->on_modify();
            } catch (const char* msg) {
                throw msg;
            }
        }


        // Initialize the database, should be called before any other methods
        //     this is for handling database connection.
        //          not for creating the database.
        static void init();

        // Cleanup the database, should be called after all other methods
        //           this will close all database connections.
        static void cleanup();

    private:
        leveldb::DB* db_;
        static leveldb::Options options_;
        ModelType type_;
        size_t size_;


        static ModelProducer* student_instance_;
        static ModelProducer* room_instance_;
        static ModelProducer* vehicle_instance_;
        static ModelProducer* room_fee_payment_instance_;
        static ModelProducer* electricity_payment_instance_;

        void dump_size() {
            if (db_) {
                leveldb::Status status = db_->Put(
                    leveldb::WriteOptions(),
                    string(SIZE_KEY),
                    to_string(size_)
                );
            }
        }

        void retrieve_size() {
            if (db_) {
                string data;
                leveldb::Status status = db_->Get(
                    leveldb::ReadOptions(),
                    string(SIZE_KEY),
                    &data
                );
                if (status.ok()) {
                    size_ = stoi(data);
                } else {
                    size_ = 0;
                    dump_size();
                }
            }
        }
};



#endif // MY_VIEW_MODEL_HPP
