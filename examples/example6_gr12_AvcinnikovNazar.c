#include <stdio.h>
#include "sqlite3.h"

int callback(void *data, int argc, char **argv, char **col_names) {
    for (int i = 0; i < argc; i++) {
        printf("%-20s: %s\n", col_names[i], argv[i] ? argv[i] : "NULL");
    }
    printf("-------------------\n");
    return 0;
}

int main() {
    sqlite3 *db;
    char *err_msg = 0;
    
    int rc = sqlite3_open("test.db", &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    
    printf("=== TABLES ===\n");
    char *sql = "SELECT name FROM sqlite_master WHERE type='table';";
    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    
    printf("\n=== SCHEMA OF Users ===\n");
    char *schema = "SELECT sql FROM sqlite_master WHERE type='table' AND name='Users';";
    rc = sqlite3_exec(db, schema, callback, 0, &err_msg);
    
    sqlite3_close(db);
    return 0;
}