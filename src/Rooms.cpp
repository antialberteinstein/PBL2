#include "objects/Rooms.hpp"

namespace rooms {

    // =====================================================================
    //                      ROOM
    // =====================================================================

    Room::Room(int id, string name, string block, int floor,
            int room, int capacity, int size, string status) {
        this->id = id;
        this->name = name;
        this->block = block;
        this->floor = floor;
        this->room = room;
        this->capacity = capacity;
        this->size = size;
        this->status = status;
    }

    int Room::get_id() {
        return this->id;
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


    // ==========================================================================
    //                          ROOM_LIST
    // ==========================================================================

    RoomList::RoomList(int capacity) {
        rooms = new Room[capacity];
        size = 0;
        this->capacity = capacity;
    }

    RoomList::~RoomList() {
        delete[] rooms;
    }

    void RoomList::insert(Room room, int index) {
        if (index < 0 || index > size) {
            index = size;
        }
        if (size == capacity) {
            return;
        }

        if (index == -1) {
            index = size;
        }
        for (int i = size; i > index; i--) {
            rooms[i] = rooms[i - 1];
        }

        rooms[index] = room;
        size++;
    }

    void RoomList::remove(int index) {
        if (index < 0 || index >= size) {
            return;
        }
        for (int i = index; i < size - 1; i++) {
            rooms[i] = rooms[i + 1];
        }
        size--;
    }

    Room& RoomList::get(int index) {
        return rooms[index];
    }

    int RoomList::get_size() {
        return size;
    }

    RoomList RoomList::from_db() {
        RoomList room_list(721);
        DATABASE db;
        STATEMENT stmt;
        string sql = "SELECT * FROM " + string(TABLE) + ";";
        if (open_db(&db)) {
            while (execute_querry(sql, db, stmt)) {
                int id = get_int(stmt, 0);
                string name = get_string(stmt, 1);
                string block = get_string(stmt, 2);
                int floor = get_int(stmt, 3);
                int room = get_int(stmt, 4);
                int capacity = get_int(stmt, 5);
                int size = get_int(stmt, 6);
                string status = get_string(stmt, 7);
                room_list.insert(Room(id, name, block, floor, room, capacity, size, status));
            }
            close_stmt(stmt);
            close_db(db);
        }
        return room_list;
    }

    void RoomList::to_db() {
        sqlite3* db;
        string sql = "DELETE FROM " + string(TABLE) + ";";
        if (sqlite3_open(DB_PATH, &db) == SQLITE_OK) {
            execute(sql, db);
            for (int i = 0; i < size; i++) {
                Room room = rooms[i];
                sql = "INSERT INTO " + string(TABLE) + " VALUES ("
                    + to_string(room.get_id()) + ", '"
                    + room.get_name() + "', '"
                    + room.get_block() + "', "
                    + to_string(room.get_floor()) + ", "
                    + to_string(room.get_room()) + ", "
                    + to_string(room.get_capacity()) + ", "
                    + to_string(room.get_size()) + ", '"
                    + room.get_status() + "');";
                execute(sql, db);
            }
            sqlite3_close(db);
        }
    }

    void RoomList::show() {
        cout << ID << " " << NAME << " " << BLOCK << " "
            << FLOOR << " " << ROOM << " " << CAPACITY << " "
            << SIZE << " " << STATUS << endl;
        for (int i = 0; i < size; i++) {
            Room room = rooms[i];
            cout << room.get_id() << " " << room.get_name() << " " << room.get_block() << " "
                << room.get_floor() << " " << room.get_room() << " " << room.get_capacity() << " "
                << room.get_size() << " " << room.get_status() << endl;
        }
    }
}