#ifndef MY_VIEW_MODEL_HPP
#define MY_VIEW_MODEL_HPP

#include "models/ModelNameFactory.hpp"
#include "objects/List.hpp"
#include "leveldb/db.h"
#include "objects/Queue.hpp"
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

        void add(unique_ptr<Model> model) {
            int key;
            bool foo = false;  // For checking if the key is already in use
            if (free_ids_.is_empty()) {
                key = size() + 1;
                foo = true;
            } else {
                key = free_ids_.dequeue();
            }

            model->set_id(to_string(key));

            leveldb::Status status = db_->Put(
                leveldb::WriteOptions(),
                model->get_id_string(),
                model->serialize()
            );

            if (!status.ok()) {
                throw runtime_error("Error adding to database: " + status.ToString());
            }
            if (foo) {
                size_++;
                dump_size();
            }
        }

        void remove(string key) {
            leveldb::Status status = db_->Delete(leveldb::WriteOptions(), key);
            if (!status.ok()) {
                throw runtime_error("Error removing from database: " + status.ToString());
            }

            // Add the id to the free list
            free_ids_.enqueue(stoi(key));
        }

        unique_ptr<Student> get_student(string key) {
            string data;
            leveldb::Status status = db_->Get(leveldb::ReadOptions(), key, &data);
            if (!status.ok()) {
                throw runtime_error("Error getting from database: " + status.ToString());
            }
            auto student = make_unique<Student>();
            student->deserialize(data);
            return move(student);
        }

        unique_ptr<Room> get_room(string key) {
            string data;
            leveldb::Status status = db_->Get(leveldb::ReadOptions(), key, &data);
            if (!status.ok()) {
                throw runtime_error("Error getting from database: " + status.ToString());
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
            string data;
            leveldb::Status status = db_->Get(leveldb::ReadOptions(), key, &data);
            if (!status.ok()) {
                throw runtime_error("Error getting from database: " + status.ToString());
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
            string data;
            leveldb::Status status = db_->Get(leveldb::ReadOptions(), key, &data);
            if (!status.ok()) {
                throw runtime_error("Error getting from database: " + status.ToString());
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
            string data;
            leveldb::Status status = db_->Get(leveldb::ReadOptions(), key, &data);
            if (!status.ok()) {
                throw runtime_error("Error getting from database: " + status.ToString());
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

        void modify(string key, unique_ptr<Model> model) {
            leveldb::Status status = db_->Put(leveldb::WriteOptions(), key, model->serialize());
            if (!status.ok()) {
                throw runtime_error("Error modifying database: " + status.ToString());
            }
        }



    private:
        leveldb::DB* db_;
        static leveldb::Options options_;
        ModelType type_;
        size_t size_;

        Queue<int> free_ids_;  // For reusing ids 

        static unique_ptr<ModelProducer> student_instance_;
        static unique_ptr<ModelProducer> room_instance_;
        static unique_ptr<ModelProducer> vehicle_instance_;
        static unique_ptr<ModelProducer> room_fee_payment_instance_;
        static unique_ptr<ModelProducer> electricity_payment_instance_;

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
