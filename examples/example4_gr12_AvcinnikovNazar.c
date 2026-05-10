#include <stdio.h>
#include "sqlite3.h"

int main() {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    
    int rc = sqlite3_open("test.db", &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    
    char *sql = "SELECT * FROM Users WHERE age > ?;";
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement\n");
        sqlite3_close(db);
        return 1;
    }
    
    sqlite3_bind_int(stmt, 1, 24);
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *name = (const char *)sqlite3_column_text(stmt, 1);
        int age = sqlite3_column_int(stmt, 2);
        
        printf("ID: %d, Name: %s, Age: %d\n", id, name, age);
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}