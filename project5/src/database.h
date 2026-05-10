#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"

sqlite3* db_open(const char *db_name);
void db_close(sqlite3 *db);
void db_create_tables(sqlite3 *db);
void db_insert_person(sqlite3 *db);
void db_select_all(sqlite3 *db);
void db_select_by_id(sqlite3 *db, int id);
void db_select_by_surname(sqlite3 *db, const char *surname_pattern);
void db_update_person(sqlite3 *db);
void db_delete_person(sqlite3 *db);
void db_insert_with_transaction(sqlite3 *db);
void show_statistics(sqlite3 *db);

#endif