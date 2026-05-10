#include <stdio.h>
#include "sqlite3.h"

int main() {
    sqlite3 *db;
    char *err_msg = 0;
    
    int rc = sqlite3_open("test.db", &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    
    char *create_sql = "CREATE TABLE IF NOT EXISTS Users ("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                       "name TEXT NOT NULL, "
                       "age INTEGER);";
    sqlite3_exec(db, create_sql, 0, 0, &err_msg);
    
    char *insert_sql = "INSERT INTO Users (name, age) VALUES "
                       "('Иван Иванов', 25), "
                       "('Мария Петрова', 30), "
                       "('Алексей Сидоров', 22);";
    
    rc = sqlite3_exec(db, insert_sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Records inserted successfully\n");
    }
    
    sqlite3_close(db);
    return 0;
}