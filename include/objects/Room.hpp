#ifndef PBL_ROOM_HPP
#define PBL_ROOM_HPP

#define ROOM_TABLE "ROOM"
#define ROOM_ID 0
#define ROOM_BLOCK 1
#define ROOM_FLOOR 2
#define ROOM_NUMBER 3
#define ROOM_CAPACITY 4
#define ROOM_CURRENT_NUMBER 5
#define ROOM_STATUS 6
#define ROOM_NULL_VALUE -1

#include <iostream>
#include <string>
#include "eins/sqlite3_support.h"

using namespace std;

class Room {
    public:
        Room(int id=ROOM_NULL_VALUE, string block="", int floor=0, int number=0,
            int capacity=0, int current_number=0, string status="");

        int get_id() const;
        string get_block() const;
        int get_floor() const;
        int get_number() const;
        int get_capacity() const;
        int get_current_number() const;
        string get_status() const;

        void set_block(string block);
        void set_floor(int floor);
        void set_number(int number);
        void set_capacity(int capacity);
        void set_current_number(int current_number);
        void set_status(string status);

        static Room from_database(sql::CURSOR& cursor);
    private:
        int id;
        string block;
        int floor;
        int number;
        int capacity;
        int current_number;
        string status;
};

#endif