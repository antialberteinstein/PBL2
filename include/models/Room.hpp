#include <iostream>
#include "models/Model.hpp"

using namespace std;

#pragma once

constexpr char room_id[] = "room_id";
constexpr char room_block[] = "block";
constexpr char room_floor[] = "floor";
constexpr char room_room_number[] = "room_number";
constexpr char room_capacity[] = "capacity";
constexpr char room_current_number[] = "current_number";
constexpr char room_status[] = "status";

class Room : public Model {
    public:

        virtual string serialize() {
            return json{
                {room_id, id},
                {room_block, block},
                {room_floor, floor},
                {room_room_number, room_number},
                {room_capacity, capacity},
                {room_current_number, current_number},
                {room_status, status},
            }.dump();
        }

        virtual void deserialize(string data) {
            auto j = json::parse(data);
            id = j[room_id];
            block = j[room_block];
            floor = j[room_floor];
            room_number = j[room_room_number];
            capacity = j[room_capacity];
            current_number = j[room_current_number];
            status = j[room_status];
        }

        string get_block() {
            return block;
        }

        int get_floor() {
            return floor;
        }

        int get_room_number() {
            return room_number;
        }

        int get_capacity() {
            return capacity;
        }

        int get_current_number() {
            return current_number;
        }

        string get_status() {
            return status;
        }

        string get_string_name() {
            return block + to_string(floor * 100 + room_number);
        }

        void set_block(const string& block) {
            this->block = block;
        }

        void set_floor(int floor) {
            this->floor = floor;
        }

        void set_room_number(int room_number) {
            this->room_number = room_number;
        }

        void set_capacity(int capacity) {
            this->capacity = capacity;
        }

        void set_current_number(int current_number) {
            this->current_number = current_number;
        }

        void set_status(const string& status) {
            this->status = status;
        }



    protected:
        string block;
        int floor;
        int room_number;
        int capacity;
        int current_number;
        string status;
};

namespace RoomStatus {
    const string AVAILABLE = "Available";
    const string FULL = "Full";
    const string MAINTENANCE = "Maintenance";
}