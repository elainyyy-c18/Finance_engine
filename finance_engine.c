#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define Transaction structure
typedef struct {
    int id;
    char date[11];      // YYYY-MM-DD
    char category[30];  // Transaction category (e.g., Food, Transport)
    double amount;      // Transaction amount
    char description[50];
} Transaction;

// Callback function for Quick Sort (Descending order by amount)
int compareTransactions(const void *a, const void *b) {
    Transaction *t1 = (Transaction *)a;
    Transaction *t2 = (Transaction *)b;
    
    if (t1->amount < t2->amount) return 1;
    if (t1->amount > t2->amount) return -1;
    return 0;
}

int main() {
    char filename[] = "data.csv";
    FILE *file = fopen(filename, "r");

    if (!file) {
        printf("Error: Cannot find %s. Please ensure the file exists.\n", filename);
        return 1;
    }

    int capacity = 100;
    int count = 0;

    // Dynamic memory allocation
    Transaction *list = (Transaction *)malloc(capacity * sizeof(Transaction));
    if (list == NULL) {
        printf("Error: Memory allocation failed.\n");
        return 1;
    }

    char line[150];
    // Skip CSV header
    fgets(line, sizeof(line), file);

    // Parse CSV data
    while (fgets(line, sizeof(line), file) && count < capacity) {
        char *token;

        token = strtok(line, ",");
        list[count].id = atoi(token);

        token = strtok(NULL, ",");
        strcpy(list[count].date, token);

        token = strtok(NULL, ",");
        strcpy(list[count].category, token);

        token = strtok(NULL, ",");
        list[count].amount = atof(token);

        token = strtok(NULL, ",");
        if (token) {
            token[strcspn(token, "\n")] = 0;
            strcpy(list[count].description, token);
        }

        count++;
    }

    fclose(file);

    printf("Successfully loaded %d transactions.\n", count);

    // Execute O(n log n) sorting
    qsort(list, count, sizeof(Transaction), compareTransactions);

    // Output financial analysis report
    printf("\n--- High Value Transactions Report (Sorted by Amount) ---\n");
    printf("%-5s | %-12s | %-15s | %-10s\n", "ID", "Date", "Category", "Amount");
    printf("-------------------------------------------------------\n");
    
    double total_expense = 0;
    for (int i = 0; i < count; i++) {
        printf("%-5d | %-12s | %-15s | %10.2f\n", 
               list[i].id, list[i].date, list[i].category, list[i].amount);
        total_expense += list[i].amount;
    }

    printf("-------------------------------------------------------\n");
    printf("Total Expense: %.2f\n", total_expense);
    printf("Average Expense: %.2f\n", count > 0 ? total_expense / count : 0);

    // Free allocated memory
    free(list);
    printf("\nMemory successfully freed. Program terminated.\n");

    return 0;
}