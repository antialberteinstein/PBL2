#include "objects/Rooms.hpp"

namespace rooms {

    // =====================================================================
    //                      ROOM
    // =====================================================================

    Room::Room(string name, string block, int floor,
            int room, int capacity, int size, string status) {
        this->name = name;
        this->block = block;
        this->floor = floor;
        this->room = room;
        this->capacity = capacity;
        this->size = size;
        this->status = status;
    }

    string Room::get_name() {
        return this->name;
    }

    string Room::get_block() {
        return this->block;
    }

    int Room::get_floor() {
        return this->floor;
    }

    int Room::get_room() {
        return this->room;
    }

    int Room::get_capacity() {
        return this->capacity;
    }

    int Room::get_size() {
        return this->size;
    }

    string Room::get_status() {
        return this->status;
    }

}