#ifndef PBL_DB_CONNECTOR_H
#define PBL_DB_CONNECTOR_H

#include <iostream>
#include "sqlite3.h"

#define DB_PATH "res/db/pbl.db"

typedef sqlite3* DATABASE;
typedef sqlite3_stmt* STATEMENT;
typedef STATEMENT RESULT_SET;

using namespace std;

bool open_db(DATABASE* db);

void close_db(DATABASE db);

void close_stmt(STATEMENT stmt);

/* Return true when executing complete. */
bool execute(const string& sql, DATABASE db);

/* Return true when executing complete, create statement. */
bool execute_query(const string& sql, DATABASE db, STATEMENT& stmt);

/* Load each row in sqlite3 table. */
bool load_row(RESULT_SET rs);

/* Get integer value */
int get_int(STATEMENT stmt, int index);

/* Get string value */
string get_string(STATEMENT stmt, int index);

/* Get double value */
double get_double(STATEMENT stmt, int index);

#endif