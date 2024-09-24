#ifndef PBL_ROOMS_H
#define PBL_ROOMS_H

#include <iostream>
#include <string>
#include "db_connector.h"

using namespace std;

namespace rooms {
    #define TABLE "rooms"
    #define ID "id"
    #define NAME "MaPhong"   // Ma phong.
    #define BLOCK "Khu"      // Khu.
    #define FLOOR "Tang"      // Tang.
    #define ROOM "Phong"      // Phong.
    #define CAPACITY "SoNguoiToiDa" // So nguoi toi da.
    #define SIZE "SoNguoiHienTai" // So nguoi hien tai.
    #define STATUS "TinhTrang" // Tinh trang.

    class Room {
        private:
            int id;
            string name;
            string block;
            int floor;
            int room;
            int capacity;
            int size;
            string status;
        public:
            Room(int id=-1, string name="", string block="",
                int floor=-1, int room=-1, int capacity=6, int size=0, string status="");
            int get_id();
            string get_name();
            string get_block();
            int get_floor();
            int get_room();
            int get_capacity();
            int get_size();
            string get_status();
    };

    class RoomList {
        private:
            Room* rooms;
            int size;
            int capacity;
        public:
            RoomList(int capacity=10);
            ~RoomList();
            void insert(Room room, int index=-1);
            void remove(int index);
            Room& get(int index);
            int get_size();

            static RoomList from_db();
            void to_db();

            void show();
    };
}

#endif