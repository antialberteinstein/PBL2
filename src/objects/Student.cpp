#include "objects/Student.hpp"

Student::Student(int id, string name, Date dob,
                 string phone_number, string email,
                 string university, string major,
                 string hometown, Date date_joined,
                 bool is_reservation, int room_capacity,
                 int room_id, string gender) : id(id), name(name), dob(dob),
                                phone_number(phone_number), email(email),
                                university(university), major(major),
                                hometown(hometown), date_joined(date_joined),
                                is_reservation(is_reservation), room_capacity(room_capacity),
                                room_id(room_id), gender(gender) {}

int Student::get_id() const {
    return id;
}

string Student::get_name() const {
    return name;
}

Date Student::get_dob() const {
    return dob;
}

string Student::get_phone_number() const {
    return phone_number;
}

string Student::get_email() const {
    return email;
}

string Student::get_university() const {
    return university;
}

string Student::get_major() const {
    return major;
}

string Student::get_hometown() const {
    return hometown;
}

Date Student::get_date_joined() const {
    return date_joined;
}

bool Student::get_is_reservation() const {
    return is_reservation;
}

int Student::get_room_capacity() const {
    return room_capacity;
}

int Student::get_room_id() const {
    return room_id;
}

string Student::get_gender() const {
    return gender;
}

void Student::set_name(string name) {
    this->name = name;
}

void Student::set_dob(Date dob) {
    this->dob = dob;
}

void Student::set_phone_number(string phone_number) {
    this->phone_number = phone_number;
}

void Student::set_email(string email) {
    this->email = email;
}

void Student::set_university(string university) {
    this->university = university;
}

void Student::set_major(string major) {
    this->major = major;
}

void Student::set_hometown(string hometown) {
    this->hometown = hometown;
}

void Student::set_room_id(int room_id) {
    this->room_id = room_id;
}

void Student::set_is_reservation(bool is_reservation) {
    this->is_reservation = is_reservation;
}

void Student::set_room_capacity(int room_capacity) {
    this->room_capacity = room_capacity;
}

void Student::set_gender(const string& gender) {
    this->gender = gender;
}

Student Student::from_database(sql::CURSOR& cursor) {
    int id = sql::get_int(cursor, STUDENT_ID);
    string name = reinterpret_cast<const char*>(sql::get_text(cursor, STUDENT_NAME));
    Date dob = sql::get_date(cursor, STUDENT_DOB);
    string phone_number = reinterpret_cast<const char*>(sql::get_text(cursor, STUDENT_PHONE_NUMBER));
    string email = reinterpret_cast<const char*>(sql::get_text(cursor, STUDENT_EMAIL));
    string university = reinterpret_cast<const char*>(sql::get_text(cursor, STUDENT_UNIVERSITY));
    string major = reinterpret_cast<const char*>(sql::get_text(cursor, STUDENT_MAJOR));
    string hometown = reinterpret_cast<const char*>(sql::get_text(cursor, STUDENT_HOMETOWN));
    Date date_joined = sql::get_date(cursor, STUDENT_DATE_JOINED);
    bool is_reservation = sql::get_bool(cursor, STUDENT_IS_RESERVATION);
    int room_capacity = sql::get_int(cursor, STUDENT_ROOM_CAPACITY);
    int room_id = sql::get_int(cursor, STUDENT_ROOM_ID);
    string gender = reinterpret_cast<const char*>(sql::get_text(cursor, STUDENT_GENDER));

    return Student(id, name, dob, phone_number, email, university,
        major, hometown, date_joined, is_reservation, room_capacity, room_id, gender);
}

bool Student::database_insert(sql::CONNECTION& connection, Student new_student) {
    string query_str = "INSERT INTO " + std::string(STUDENT_TABLE)
        + " (name, dob, phone_number, email, university, major, hometown, date_joined, is_reservation, room_capacity, room_id, gender)"
        + "VALUES("
        + "'" + new_student.get_name() + "', "
        + "'" + DateConverter::to_string(new_student.get_dob()) + "', "
        + "'" + new_student.get_phone_number() + "', "
        + "'" + new_student.get_email() + "', "
        + "'" + new_student.get_university() + "', "
        + "'" + new_student.get_major() + "', "
        + "'" + new_student.get_hometown() + "', "
        + "'" + DateConverter::to_string(new_student.get_date_joined()) + "', "
        + std::string((new_student.get_is_reservation()) ? "1" : "0") + ", "
        + std::to_string(new_student.get_room_capacity()) + ", "
        + std::to_string(new_student.get_room_id()) + ", "
        + "'" + new_student.get_gender() + "'"
            + ");";
    sql::QUERY query = sql::from_string(query_str);

    sql::prepare(connection, query);
    if (sql::get_statement() == nullptr) {
        return false;
    }

    sql::run(sql::get_statement());

    return true;
}

bool Student::database_update(sql::CONNECTION& connection, Student updated_student) {
    string query_str = "UPDATE " + std::string(STUDENT_TABLE)
        + " SET name = '" + updated_student.get_name() + "', "
        + "dob = '" + DateConverter::to_string(updated_student.get_dob()) + "', "
        + "phone_number = '" + updated_student.get_phone_number() + "', "
        + "email = '" + updated_student.get_email() + "', "
        + "university = '" + updated_student.get_university() + "', "
        + "major = '" + updated_student.get_major() + "', "
        + "hometown = '" + updated_student.get_hometown() + "', "
        + "date_joined = '" + DateConverter::to_string(updated_student.get_date_joined()) + "', "
        + "is_reservation = " + std::string((updated_student.get_is_reservation()) ? "1" : "0") + ", "
        + "room_capacity = " + std::to_string(updated_student.get_room_capacity()) + ", "
        + "room_id = " + std::to_string(updated_student.get_room_id()) + ", "
        + "gender = '" + updated_student.get_gender() + "'"
        + " WHERE id = " + std::to_string(updated_student.get_id()) + ";";
    sql::QUERY query = sql::from_string(query_str);

    sql::prepare(connection, query);
    
    if (sql::get_statement() == nullptr) {
        return false;
    }

    sql::run(sql::get_statement());
    return true;
}
