#ifndef PBL_SQLITE3_SUPPORT_HPP
#define PBL_SQLITE3_SUPPORT_HPP

#include "sqlite3.h"
#include <iostream>
#include "objects/Date.hpp"
#include <exception>

#define DB_PATH "res/db/pbl.db"

namespace sql {
    typedef sqlite3* DATABASE;
    typedef sqlite3* CONNECTION;
    typedef sqlite3_stmt* STATEMENT;
    typedef sqlite3_stmt* CURSOR;
    typedef const char* QUERY;
    typedef const unsigned char* TEXT;
    typedef int INT;
    typedef int BOOL;
    typedef double DOUBLE;
    typedef Date DATE;

    void connect(const std::string& db_path);
    bool check_connection();
    void reconnect();
    void disconnect();
    CONNECTION& get_connection();

    void prepare(CONNECTION& connection, QUERY query);
    STATEMENT& get_statement();
    void finalize_statement();

    bool next_row(CURSOR& cursor);

    bool run(CURSOR& cursor);

    TEXT get_text(CURSOR& cursor, INT column);
    INT get_int(CURSOR& cursor, INT column);
    DOUBLE get_double(CURSOR& cursor, INT column);
    BOOL get_bool(CURSOR& cursor, INT column);
    DATE get_date(CURSOR& cursor, INT column);

    QUERY from_string(const std::string& str);

    void bind_text(STATEMENT& stmt, INT index, const std::string& value);
    void bind_int(STATEMENT& stmt, INT index, INT value);
    void bind_double(STATEMENT& stmt, INT index, DOUBLE value);
    void bind_bool(STATEMENT& stmt, INT index, BOOL value);
    void bind_date(STATEMENT& stmt, INT index, DATE value);

    class CreatingConnectionException : public std::exception {
        public:
            CreatingConnectionException(const std::string& message="") : message(message) {}

            std::string get_message() {
                return "Error creating connection: " + message + "!!";
            }
        private:
            std::string message;
    };

    class ExecutingQueryException : public std::exception {
        public:
            ExecutingQueryException(const std::string& message="") : message(message) {}
            std::string get_message() {
                return "Error executing query: " + message + "!!";
            }
        private:
            std::string message;
    };
}

#endif