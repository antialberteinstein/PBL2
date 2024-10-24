#include "objects/Room.hpp"

Room::Room(int id, string block, int floor, int number, int capacity, int current_number, string status)
    : id(id), block(block), floor(floor), number(number), capacity(capacity), current_number(current_number), status(status) {}

int Room::get_id() const {
    return id;
}

string Room::get_block() const {
    return block;
}

int Room::get_floor() const {
    return floor;
}

int Room::get_number() const {
    return number;
}

int Room::get_capacity() const {
    return capacity;
}

int Room::get_current_number() const {
    return current_number;
}

string Room::get_status() const {
    return status;
}

void Room::set_block(string block) {
    this->block = block;
}

void Room::set_floor(int floor) {
    this->floor = floor;
}

void Room::set_number(int number) {
    this->number = number;
}

void Room::set_capacity(int capacity) {
    this->capacity = capacity;
}

void Room::set_current_number(int current_number) {
    this->current_number = current_number;
}

void Room::set_status(string status) {
    this->status = status;
}

Room Room::from_database(sql::CURSOR& cursor) {
    int id = sql::get_int(cursor, ROOM_ID);
    string block = reinterpret_cast<const char*>(sql::get_text(cursor, ROOM_BLOCK));
    int floor = sql::get_int(cursor, ROOM_FLOOR);
    int number = sql::get_int(cursor, ROOM_NUMBER);
    int capacity = sql::get_int(cursor, ROOM_CAPACITY);
    int current_number = sql::get_int(cursor, ROOM_CURRENT_NUMBER);
    string status = reinterpret_cast<const char*>(sql::get_text(cursor, ROOM_STATUS));
    return Room(id, block, floor, number, capacity, current_number, status);
}