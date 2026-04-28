#include "finance_engine.h"

int compareByAmount(const void *a, const void *b) {
    Transaction *t1 = (Transaction *)a;
    Transaction *t2 = (Transaction *)b;
    if (t1->amount < t2->amount) return 1;
    if (t1->amount > t2->amount) return -1;
    return 0;
}

int compareByDate(const void *a, const void *b) {
    Transaction *t1 = (Transaction *)a;
    Transaction *t2 = (Transaction *)b;
    return strcmp(t2->date, t1->date);
}

int compareByCategory(const void *a, const void *b) {
    Transaction *t1 = (Transaction *)a;
    Transaction *t2 = (Transaction *)b;
    return strcmp(t1->category, t2->category);
}

void generateCategorySummary(Transaction *list, int count) {
    if (count == 0) return;
    qsort(list, count, sizeof(Transaction), compareByCategory);

    printf("\n--- Category Financial Summary ---\n");
    printf("%-15s | %-10s\n", "Category", "Total Amount");
    printf("----------------------------------\n");

    double category_total = 0;
    for (int i = 0; i < count; i++) {
        category_total += list[i].amount;
        if (i == count - 1 || strcmp(list[i].category, list[i + 1].category) != 0) {
            printf("%-15s | %10.2f\n", list[i].category, category_total);
            category_total = 0;
        }
    }
    printf("----------------------------------\n");
}

void exportToCSV(Transaction *list, int count) {
    FILE *file = fopen("sorted_export.csv", "w");
    if (!file) return;
    fprintf(file, "id,date,category,amount,description\n");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%s,%s,%.2f,%s\n", 
                list[i].id, list[i].date, list[i].category, list[i].amount, list[i].description);
    }
    fclose(file);
    printf("\n>>> SUCCESS: Exported %d records to 'sorted_export.csv' <<<\n", count);
}