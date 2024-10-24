#include "eins/sqlite3_support.h"

sql::CONNECTION main_connection;
sql::STATEMENT main_statement;

std::string main_database_path;

void sql::connect(const std::string& db_path) {
    main_database_path = db_path;
    int exit = sqlite3_open(db_path.c_str(), &main_connection);
    if (exit) {
        throw CreatingConnectionException(sqlite3_errmsg(main_connection));
    }
}

sql::CONNECTION& sql::get_connection() {
    return main_connection;
}

void sql::disconnect() {
    sqlite3_finalize(main_statement);
    sqlite3_close(main_connection);
    main_connection = nullptr;
}

bool sql::check_connection() {
    return main_connection != nullptr;
}

void sql::reconnect() {
    disconnect();
    connect(main_database_path);
}

void sql::prepare(sql::CONNECTION& connection, sql::QUERY query) {
    int exit = sqlite3_prepare_v2(connection, query, -1, &main_statement, 0);
    if (exit != SQLITE_OK) {
        throw ExecutingQueryException(sqlite3_errmsg(connection));
    }
}

sql::STATEMENT& sql::get_statement() {
    return main_statement;
}

void sql::finalize_statement() {
    sqlite3_finalize(main_statement);
}

bool sql::next_row(sql::CURSOR& cursor) {
    return sqlite3_step(cursor) == SQLITE_ROW;
}

bool sql::run(sql::CURSOR& cursor) {
    return next_row(cursor);
}

sql::TEXT sql::get_text(sql::CURSOR& cursor, sql::INT column) {
    return sqlite3_column_text(cursor, column);
}

sql::INT sql::get_int(sql::CURSOR& cursor, sql::INT column) {
    return sqlite3_column_int(cursor, column);
}

sql::DOUBLE sql::get_double(sql::CURSOR& cursor, sql::INT column) {
    return sqlite3_column_double(cursor, column);
}

sql::BOOL sql::get_bool(sql::CURSOR& cursor, sql::INT column) {
    return sqlite3_column_int(cursor, column);
}

sql::DATE sql::get_date(sql::CURSOR& cursor, sql::INT column) {
    const unsigned char* date = sqlite3_column_text(cursor, column);
    return DateConverter::from_string(reinterpret_cast<const char*>(date));
}

sql::QUERY sql::from_string(const std::string& str) {
    static std::string _query = str;
    sql::QUERY query = _query.c_str();
    return query;
}

void sql::bind_text(sql::STATEMENT& stmt, sql::INT index, const std::string& value) {
    sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_STATIC);
}

void sql::bind_int(sql::STATEMENT& stmt, sql::INT index, sql::INT value) {
    sqlite3_bind_int(stmt, index, value);
}

void sql::bind_double(sql::STATEMENT& stmt, sql::INT index, sql::DOUBLE value) {
    sqlite3_bind_double(stmt, index, value);
}

void sql::bind_bool(sql::STATEMENT& stmt, sql::INT index, sql::BOOL value) {
    sqlite3_bind_int(stmt, index, value);
}

void sql::bind_date(sql::STATEMENT& stmt, sql::INT index, sql::DATE value) {
    sqlite3_bind_text(stmt, index, DateConverter::to_string(value).c_str(), -1, SQLITE_STATIC);
}