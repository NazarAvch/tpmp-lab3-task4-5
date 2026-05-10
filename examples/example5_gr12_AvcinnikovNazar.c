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
    
    sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, &err_msg);
    
    char *insert1 = "INSERT INTO Users (name, age) VALUES ('Транзакция1', 40);";
    char *insert2 = "INSERT INTO Users (name, age) VALUES ('Транзакция2', 45);";
    
    rc = sqlite3_exec(db, insert1, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_exec(db, "ROLLBACK;", 0, 0, 0);
        sqlite3_close(db);
        return 1;
    }
    
    rc = sqlite3_exec(db, insert2, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_exec(db, "ROLLBACK;", 0, 0, 0);
        sqlite3_close(db);
        return 1;
    }
    
    sqlite3_exec(db, "COMMIT;", 0, 0, &err_msg);
    
    printf("Transaction completed successfully\n");
    sqlite3_close(db);
    return 0;
}