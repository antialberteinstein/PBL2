#include "eins/db_connector.h"

bool open_db(DATABASE* db) {
    return sqlite3_open(DB_PATH, db) == SQLITE_OK;
}

void close_db(DATABASE db) {
    sqlite3_close(db);
}

void close_stmt(STATEMENT stmt) {
    sqlite3_finalize(stmt);
}

bool execute(const string& sql, DATABASE db) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool execute_query(const string& sql, DATABASE db, STATEMENT& stmt) {
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << sqlite3_errmsg(db) << endl;
        close_db(db);
        return false;
    }
    return true;
}

bool load_row(RESULT_SET rs) {
    return sqlite3_step(rs) == SQLITE_ROW;
}

int get_int(STATEMENT stmt, int index) {
    return sqlite3_column_int(stmt, index);
}

string get_string(STATEMENT stmt, int index) {
    return string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index)));
}

double get_double(STATEMENT stmt, int index) {
    return sqlite3_column_double(stmt, index);
}