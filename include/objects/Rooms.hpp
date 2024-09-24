#ifndef PBL_ROOMS_H
#define PBL_ROOMS_H

#include <iostream>
#include <string>
#include "eins/db_connector.h"

using namespace std;

namespace rooms {
    #define TABLE "rooms"

    #define NAME "MaPhong"   // Ma phong.
    #define INDEX_NAME 0

    #define BLOCK "Khu"      // Khu.
    #define INDEX_BLOCK 1

    #define FLOOR "Tang"      // Tang.
    #define INDEX_FLOOR 2

    #define ROOM "Phong"      // Phong.
    #define INDEX_ROOM 3

    #define CAPACITY "SoNguoiToiDa" // So nguoi toi da.
    #define INDEX_CAPACITY 4

    #define SIZE "SoNguoiHienTai" // So nguoi hien tai.
    #define INDEX_SIZE 5

    #define STATUS "TinhTrang" // Tinh trang.
    #define INDEX_STATUS 6

    class Room {
        private:
            string name;
            string block;
            int floor;
            int room;
            int capacity;
            int size;
            string status;
        public:
            Room(string name="", string block="",
                int floor=-1, int room=-1, int capacity=6, int size=0, string status="");
            string get_name();
            string get_block();
            int get_floor();
            int get_room();
            int get_capacity();
            int get_size();
            string get_status();
    };
}

#endif