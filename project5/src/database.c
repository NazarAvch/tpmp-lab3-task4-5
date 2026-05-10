#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

int callback(void *data, int argc, char **argv, char **col_names) {
    for (int i = 0; i < argc; i++) {
        printf("%-20s", col_names[i]);
    }
    printf("\n");
    
    for (int i = 0; i < argc; i++) {
        printf("%-20s", argv[i] ? argv[i] : "NULL");
    }
    printf("\n\n");
    return 0;
}

sqlite3* db_open(const char *db_name) {
    sqlite3 *db;
    int rc = sqlite3_open(db_name, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    return db;
}

void db_close(sqlite3 *db) {
    sqlite3_close(db);
}

void db_create_tables(sqlite3 *db) {
    char *err_msg = 0;
    
    char *sql_person = 
        "CREATE TABLE IF NOT EXISTS person ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "surname TEXT NOT NULL, "
        "name TEXT NOT NULL, "
        "patronymic TEXT, "
        "gender TEXT, "
        "nationality TEXT, "
        "height REAL, "
        "weight REAL, "
        "birth_year INTEGER, "
        "birth_month INTEGER, "
        "birth_day INTEGER, "
        "phone TEXT, "
        "zip_code TEXT, "
        "country TEXT, "
        "region TEXT, "
        "district TEXT, "
        "city TEXT, "
        "street TEXT, "
        "house TEXT, "
        "apartment TEXT, "
        "position_id INTEGER, "
        "photo BLOB"
        ");";
    
    sqlite3_exec(db, sql_person, 0, 0, &err_msg);
    
    char *sql_category = 
        "CREATE TABLE IF NOT EXISTS category ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "position_name TEXT NOT NULL, "
        "position_description TEXT"
        ");";
    
    sqlite3_exec(db, sql_category, 0, 0, &err_msg);
    
    printf("Tables created successfully\n");
}

void db_insert_person(sqlite3 *db) {
    char surname[50], name[50], patronymic[50], gender[5], nationality[50];
    float height, weight;
    int birth_year, birth_month, birth_day;
    char phone[20], zip_code[10], country[50], region[50], district[50];
    char city[50], street[50], house[10], apartment[10];
    
    printf("Введите фамилию: "); scanf("%s", surname);
    printf("Введите имя: "); scanf("%s", name);
    printf("Введите отчество: "); scanf("%s", patronymic);
    printf("Введите пол (М/Ж): "); scanf("%s", gender);
    printf("Введите национальность: "); scanf("%s", nationality);
    printf("Введите рост: "); scanf("%f", &height);
    printf("Введите вес: "); scanf("%f", &weight);
    printf("Введите год рождения: "); scanf("%d", &birth_year);
    printf("Введите месяц рождения: "); scanf("%d", &birth_month);
    printf("Введите день рождения: "); scanf("%d", &birth_day);
    printf("Введите телефон: "); scanf("%s", phone);
    printf("Введите почтовый индекс: "); scanf("%s", zip_code);
    printf("Введите страну: "); scanf("%s", country);
    printf("Введите область: "); scanf("%s", region);
    printf("Введите район: "); scanf("%s", district);
    printf("Введите город: "); scanf("%s", city);
    printf("Введите улицу: "); scanf("%s", street);
    printf("Введите дом: "); scanf("%s", house);
    printf("Введите квартиру: "); scanf("%s", apartment);
    
    char sql[1000];
    sprintf(sql, 
        "INSERT INTO person (surname, name, patronymic, gender, nationality, "
        "height, weight, birth_year, birth_month, birth_day, phone, zip_code, "
        "country, region, district, city, street, house, apartment) "
        "VALUES ('%s', '%s', '%s', '%s', '%s', %.1f, %.1f, %d, %d, %d, "
        "'%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');",
        surname, name, patronymic, gender, nationality, height, weight,
        birth_year, birth_month, birth_day, phone, zip_code, country,
        region, district, city, street, house, apartment);
    
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Запись добавлена успешно (autocommit)\n");
    }
}

void db_select_all(sqlite3 *db) {
    char *sql = "SELECT id, surname, name, patronymic, birth_year, city FROM person;";
    char *err_msg = 0;
    printf("\n=== Все записи ===\n");
    sqlite3_exec(db, sql, callback, 0, &err_msg);
}

void db_select_by_id(sqlite3 *db, int id) {
    sqlite3_stmt *stmt;
    char *sql = "SELECT * FROM person WHERE id = ?;";
    
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, id);
    
    printf("\n=== Поиск по ID = %d ===\n", id);
    int cols = sqlite3_column_count(stmt);
    int has_result = 0;
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        has_result = 1;
        for (int i = 0; i < cols; i++) {
            printf("%s: %s\n", sqlite3_column_name(stmt, i), 
                   sqlite3_column_text(stmt, i));
        }
    }
    
    if (!has_result) {
        printf("Запись с ID=%d не найдена\n", id);
    }
    
    sqlite3_finalize(stmt);
}

void db_select_by_surname(sqlite3 *db, const char *pattern) {
    char sql[200];
    sprintf(sql, "SELECT * FROM person WHERE surname LIKE '%s';", pattern);
    
    char *err_msg = 0;
    printf("\n=== Поиск по маске '%s' ===\n", pattern);
    sqlite3_exec(db, sql, callback, 0, &err_msg);
}

void db_update_person(sqlite3 *db) {
    int id;
    float new_weight;
    printf("Введите ID человека для обновления веса: ");
    scanf("%d", &id);
    printf("Введите новый вес: ");
    scanf("%f", &new_weight);
    
    char sql[200];
    sprintf(sql, "UPDATE person SET weight = %.1f WHERE id = %d;", new_weight, id);
    
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Данные обновлены\n");
    }
}

void db_delete_person(sqlite3 *db) {
    int id;
    printf("Введите ID человека для удаления: ");
    scanf("%d", &id);
    
    char sql[100];
    sprintf(sql, "DELETE FROM person WHERE id = %d;", id);
    
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Запись удалена\n");
    }
}

void db_insert_with_transaction(sqlite3 *db) {
    char *err_msg = 0;
    
    sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, &err_msg);
    
    printf("Вставка 3 записей в транзакции...\n");
    
    char *inserts[] = {
        "INSERT INTO person (surname, name, gender, nationality, height, weight, "
        "birth_year, birth_month, birth_day, phone, country, city, street, house, apartment) "
        "VALUES ('Транзактов1', 'Иван', 'М', 'русский', 175.0, 70.0, 1990, 5, 10, "
        "'+375290000001', 'Беларусь', 'Минск', 'Ленина', '1', '1');",
        
        "INSERT INTO person (surname, name, gender, nationality, height, weight, "
        "birth_year, birth_month, birth_day, phone, country, city, street, house, apartment) "
        "VALUES ('Транзактов2', 'Петр', 'М', 'белорус', 180.0, 75.0, 1992, 7, 15, "
        "'+375290000002', 'Беларусь', 'Минск', 'Пушкина', '2', '2');",
        
        "INSERT INTO person (surname, name, gender, nationality, height, weight, "
        "birth_year, birth_month, birth_day, phone, country, city, street, house, apartment) "
        "VALUES ('Транзактов3', 'Анна', 'Ж', 'украинка', 165.0, 58.0, 1995, 3, 20, "
        "'+375290000003', 'Беларусь', 'Минск', 'Гагарина', '3', '3');"
    };
    
    int rc;
    for (int i = 0; i < 3; i++) {
        rc = sqlite3_exec(db, inserts[i], 0, 0, &err_msg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Ошибка в транзакции: %s\n", err_msg);
            sqlite3_exec(db, "ROLLBACK;", 0, 0, 0);
            printf("Транзакция отменена (ROLLBACK)\n");
            return;
        }
    }
    
    sqlite3_exec(db, "COMMIT;", 0, 0, &err_msg);
    printf("Транзакция успешно завершена (COMMIT)\n");
}

void show_statistics(sqlite3 *db) {
    char *err_msg = 0;
    
    printf("\n=== СТАТИСТИКА ===\n");
    
    char *sql1 = "SELECT surname, name, birth_year FROM person "
                 "ORDER BY birth_year DESC LIMIT 1;";
    printf("\nСамый молодой:\n");
    sqlite3_exec(db, sql1, callback, 0, &err_msg);
    
    char *sql2 = "SELECT COUNT(*) AS count FROM person WHERE height > 164;";
    printf("Количество людей с ростом > 164 см:\n");
    sqlite3_exec(db, sql2, callback, 0, &err_msg);
    
    char *sql3 = "SELECT MAX(height) AS max_height, MIN(height) AS min_height FROM person;";
    printf("Максимальный и минимальный рост:\n");
    sqlite3_exec(db, sql3, callback, 0, &err_msg);
    
    char *sql4 = "SELECT SUM(weight) AS total_weight FROM person WHERE weight < 76;";
    printf("Суммарный вес (вес < 76 кг):\n");
    sqlite3_exec(db, sql4, callback, 0, &err_msg);
}