#ifndef PBL_STUDENT_HPP
#define PBL_STUDENT_HPP

#include <iostream>
#include <string>
#include "objects/Date.hpp"
#include "eins/sqlite3_support.h"

using namespace std;

#define STUDENT_TABLE "STUDENT"
#define STUDENT_ID 0
#define STUDENT_NAME 1
#define STUDENT_DOB 2
#define STUDENT_PHONE_NUMBER 3
#define STUDENT_EMAIL 4
#define STUDENT_UNIVERSITY 5
#define STUDENT_MAJOR 6
#define STUDENT_HOMETOWN 7
#define STUDENT_DATE_JOINED 8
#define STUDENT_IS_RESERVATION 9
#define STUDENT_ROOM_CAPACITY 10
#define STUDENT_ROOM_ID 11
#define STUDENT_GENDER 12
#define STUDENT_NULL_VALUE -1  // when a student created, the room_id is -1.


class Student {
    public:
        Student(int id=STUDENT_NULL_VALUE, string name="", Date dob=Date(0, 0, 0),
                string phone_number="", string email="",
                string university="", string major="",
                string hometown="", Date date_joined=Date(0, 0, 0),
                bool is_reservation=false, int room_capacity=0,
                int room_id=STUDENT_NULL_VALUE, string gender="");

        int get_id() const;
        string get_name() const;
        Date get_dob() const;
        string get_phone_number() const;
        string get_email() const;
        string get_university() const;
        string get_major() const;
        string get_hometown() const;
        Date get_date_joined() const;
        bool get_is_reservation() const;
        int get_room_capacity() const;
        int get_room_id() const;
        string get_gender() const;

        void set_name(string name);
        void set_dob(Date dob);
        void set_phone_number(string phone_number);
        void set_email(string email);
        void set_university(string university);
        void set_major(string major);
        void set_hometown(string hometown);
        void set_room_id(int room_id);
        void set_is_reservation(bool is_reservation);
        void set_room_capacity(int room_capacity);
        void set_gender(const string& gender);


        static Student from_database(sql::CURSOR& cursor);

        // Return true if the student is successfully inserted into the database.
        static bool database_insert(sql::CONNECTION& connection, Student new_student);

        // Return true if the student is successfully updated in the database.
        static bool database_update(sql::CONNECTION& connection, Student updated_student);
    private:
        int id;
        string name;
        Date dob;
        string phone_number;
        string email;
        string university;
        string major;
        string hometown;
        Date date_joined;
        bool is_reservation;
        int room_capacity;
        int room_id;
        string gender;
};

#endif
