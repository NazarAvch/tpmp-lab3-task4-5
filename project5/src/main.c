#include <stdio.h>
#include <stdlib.h>
#include "database.h"

void print_menu() {
    printf("\n========================================\n");
    printf("  БАЗА ДАННЫХ «ЧЕЛОВЕК»\n");
    printf("========================================\n");
    printf("1.  Вывести все записи\n");
    printf("2.  Найти по ID\n");
    printf("3.  Найти по фамилии (по маске)\n");
    printf("4.  Добавить нового человека (autocommit)\n");
    printf("5.  Обновить вес человека\n");
    printf("6.  Удалить человека по ID\n");
    printf("7.  Добавить 3 записи (транзакция)\n");
    printf("8.  Вывести статистику\n");
    printf("0.  Выход\n");
    printf("========================================\n");
    printf("Выберите действие: ");
}

int main() {
    sqlite3 *db = db_open("data/people.db");
    if (!db) {
        return 1;
    }
    
    db_create_tables(db);
    
    int choice;
    int id;
    char surname[50];
    
    do {
        print_menu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                db_select_all(db);
                break;
            case 2:
                printf("Введите ID: ");
                scanf("%d", &id);
                db_select_by_id(db, id);
                break;
            case 3:
                printf("Введите начало фамилии (например, И%%): ");
                scanf("%s", surname);
                db_select_by_surname(db, surname);
                break;
            case 4:
                db_insert_person(db);
                break;
            case 5:
                db_update_person(db);
                break;
            case 6:
                db_delete_person(db);
                break;
            case 7:
                db_insert_with_transaction(db);
                break;
            case 8:
                show_statistics(db);
                break;
            case 0:
                printf("Выход...\n");
                break;
            default:
                printf("Неверный выбор!\n");
        }
    } while (choice != 0);
    
    db_close(db);
    return 0;
}