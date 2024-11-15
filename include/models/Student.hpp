#include <iostream>
#include "models/Model.hpp"
#include "models/Room.hpp"
using namespace std;

#pragma once

#define NO_ROOM_ID ""

constexpr char student_name[] = "name";
constexpr char student_id[] = "id";
constexpr char student_dob[] = "dob";
constexpr char student_gender[] = "gender";
constexpr char student_phone_number[] = "phone_number";
constexpr char student_email[] = "email";
constexpr char student_university[] = "university";
constexpr char student_major[] = "major";
constexpr char student_hometown[] = "hometown";
constexpr char student_date_joined[] = "date_joined";
constexpr char student_room_id[] = "room_id";

class Student : public Model{
    public:

        virtual string serialize() {
            return json{
                {student_id, id},
                {student_name, name},
                {student_dob, dob},
                {student_gender, gender},
                {student_phone_number, phone_number},
                {student_email, email},
                {student_university, university},
                {student_major, major},
                {student_hometown, hometown},
                {student_date_joined, date_joined},
                {student_room_id, room_id},
            }.dump();
        }

        virtual void deserialize(string data) {
            auto j = json::parse(data);
            id = j[student_id];
            name = j[student_name];
            dob = j[student_dob];
            gender = j[student_gender];
            phone_number = j[student_phone_number];
            email = j[student_email];
            university = j[student_university];
            major = j[student_major];
            hometown = j[student_hometown];
            date_joined = j[student_date_joined];
            room_id = j[student_room_id];
        }

        string get_name() {
            return name;
        }

        string get_dob() {
            return dob;
        }

        string get_gender() {
            return gender;
        }

        string get_phone_number() {
            return phone_number;
        }

        string get_email() {
            return email;
        }

        string get_university() {
            return university;
        }

        string get_major() {
            return major;
        }

        string get_hometown() {
            return hometown;
        }

        string get_date_joined() {
            return date_joined;
        }

        string get_room_id() {
            return room_id;
        }

        string get_room();

        void set_name(const string& name) {
            this->name = name;
        }

        void set_dob(const string& dob) {
            this->dob = dob;
        }

        void set_gender(const string& gender) {
            this->gender = gender;
        }

        void set_phone_number(const string& phone_number) {
            this->phone_number = phone_number;
        }

        void set_email(const string& email) {
            this->email = email;
        }

        void set_university(const string& university) {
            this->university = university;
        }

        void set_major(const string& major) {
            this->major = major;
        }

        void set_hometown(const string& hometown) {
            this->hometown = hometown;
        }

        void set_date_joined(const string& date_joined) {
            this->date_joined = date_joined;
        }

        void set_room_id(const string& room_id) {
            this->room_id = room_id;
        }

       
        virtual string hash_to_id();    

        unique_ptr<Room> get_room_ref();

        void turn_on_creating_flag() {
            creating_flag = true;
        }

        void turn_off_creating_flag() {
            creating_flag = false;
        }

    protected:
        string name;
        string dob;
        string gender;
        string phone_number;
        string email;
        string university;
        string major;
        string hometown;
        string date_joined;
        string room_id;

    private:
        bool creating_flag;
};
