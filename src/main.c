#include "finance_engine.h"

int main() {
    FILE *file = fopen("data.csv", "r");
    if (!file) {
        printf("Error: data.csv not found.\n");
        return 1;
    }

    int capacity = 1000, count = 0;
    Transaction *list = (Transaction *)malloc(capacity * sizeof(Transaction));
    char line[200];
    fgets(line, sizeof(line), file); // Skip header

    while (fgets(line, sizeof(line), file)) {
        if (count >= capacity) {
            capacity *= 2;
            list = (Transaction *)realloc(list, capacity * sizeof(Transaction));
        }
        char *token = strtok(line, ",");
        if (!token) continue;
        list[count].id = atoi(token);
        token = strtok(NULL, ","); strcpy(list[count].date, token);
        token = strtok(NULL, ","); strcpy(list[count].category, token);
        token = strtok(NULL, ","); list[count].amount = atof(token);
        token = strtok(NULL, ",");
        if (token) {
            token[strcspn(token, "\n")] = 0;
            strcpy(list[count].description, token);
        }
        count++;
    }
    fclose(file);

    int choice;
    while (1) {
        printf("\n[1] Amount Sort [2] Date Sort [3] Category Sort\n");
        printf("[4] EXPORT CSV  [5] SUMMARY    [6] EXIT\nChoice: ");
        if (scanf("%d", &choice) != 1 || choice == 6) break;

        switch (choice) {
            case 1: qsort(list, count, sizeof(Transaction), compareByAmount); break;
            case 2: qsort(list, count, sizeof(Transaction), compareByDate); break;
            case 3: qsort(list, count, sizeof(Transaction), compareByCategory); break;
            case 4: exportToCSV(list, count); continue;
            case 5: generateCategorySummary(list, count); continue;
            default: continue;
        }

        printf("\n%-5s | %-12s | %-15s | %-10s\n", "ID", "Date", "Category", "Amount");
        int limit = (count > 15) ? 15 : count;
        for (int i = 0; i < limit; i++) 
            printf("%-5d | %-12s | %-15s | %10.2f\n", list[i].id, list[i].date, list[i].category, list[i].amount);
    }
    free(list);
    return 0;
}