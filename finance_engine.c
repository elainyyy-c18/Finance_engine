#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
    int id;
    char date[11];      // YYYY-MM-DD
    char category[30];  // Transaction category
    double amount;      // Transaction amount
    char description[50];
} Transaction;

int compareByAmount(const void *a, const void *b) 
{
    Transaction *t1 = (Transaction *)a;
    Transaction *t2 = (Transaction *)b;
    if (t1->amount < t2->amount) return 1;
    if (t1->amount > t2->amount) return -1;
    return 0;
}

int compareByDate(const void *a, const void *b) 
{
    Transaction *t1 = (Transaction *)a;
    Transaction *t2 = (Transaction *)b;
    return strcmp(t2->date, t1->date);
}

int compareByCategory(const void *a, const void *b) 
{
    Transaction *t1 = (Transaction *)a;
    Transaction *t2 = (Transaction *)b;
    return strcmp(t1->category, t2->category);
}

void exportToCSV(Transaction *list, int count) 
{
    char export_name[] = "sorted_export.csv";
    FILE *file = fopen(export_name, "w");
    
    if (!file) 
    {
        printf("\nError: Could not create export file.\n");
        return;
    }

    fprintf(file, "id,date,category,amount,description\n");

    for (int i = 0; i < count; i++) 
    {
        fprintf(file, "%d,%s,%s,%.2f,%s\n", 
                list[i].id, list[i].date, list[i].category, list[i].amount, list[i].description);
    }

    fclose(file);
    printf("\n>>> SUCCESS: Exported %d records to '%s' <<<\n", count, export_name);
}

int main() 
{
    char filename[] = "data.csv";
    FILE *file = fopen(filename, "r");

    if (!file) 
    {
        printf("Error: Cannot find %s. Please ensure the file exists.\n", filename);
        return 1;
    }

    int capacity = 1000; 
    int count = 0;
    Transaction *list = (Transaction *)malloc(capacity * sizeof(Transaction));
    
    if (list == NULL) \
    {
        printf("Error: Memory allocation failed.\n");
        fclose(file);
        return 1;
    }

    char line[150];
    fgets(line, sizeof(line), file); // Skip header

    while (fgets(line, sizeof(line), file)) \
    {
        if (count >= capacity) 
        {
            capacity *= 2; 
            Transaction *temp = (Transaction *)realloc(list, capacity * sizeof(Transaction));
            if (temp == NULL) 
            {
                free(list);
                fclose(file);
                return 1;
            }
            list = temp;
        }

        char *token = strtok(line, ",");
        if (!token) continue;
        list[count].id = atoi(token);

        token = strtok(NULL, ",");
        if (token) strcpy(list[count].date, token);

        token = strtok(NULL, ",");
        if (token) strcpy(list[count].category, token);

        token = strtok(NULL, ",");
        if (token) list[count].amount = atof(token);

        token = strtok(NULL, ",");
        if (token) 
        {
            token[strcspn(token, "\n")] = 0;
            strcpy(list[count].description, token);
        }
        count++;
    }
    fclose(file);

    int choice;
    while (1) 
    {
        printf("\n=========================================\n");
        printf("    Financial Data Processing Engine     \n");
        printf("=========================================\n");
        printf(" [1] Sort by Amount (High to Low)\n");
        printf(" [2] Sort by Date (Newest to Oldest)\n");
        printf(" [3] Sort by Category (A to Z)\n");
        printf(" [4] EXPORT current results to CSV\n");
        printf(" [5] Exit Program\n");
        printf("-----------------------------------------\n");
        printf("Please enter your choice (1-5): ");
        
        if (scanf("%d", &choice) != 1) 
        {
            printf("\nInvalid input. Exiting...\n");
            break;
        }

        if (choice == 5) break;

        switch (choice) 
        {
            case 1: qsort(list, count, sizeof(Transaction), compareByAmount); printf("\nSorted by Amount.\n"); break;
            case 2: qsort(list, count, sizeof(Transaction), compareByDate); printf("\nSorted by Date.\n"); break;
            case 3: qsort(list, count, sizeof(Transaction), compareByCategory); printf("\nSorted by Category.\n"); break;
            case 4: exportToCSV(list, count); continue; 
            default: printf("\nInvalid choice.\n"); continue;
        }

        printf("\n%-5s | %-12s | %-15s | %-10s\n", "ID", "Date", "Category", "Amount");
        printf("-------------------------------------------------------\n");
        int limit = (count > 15) ? 15 : count;
        for (int i = 0; i < limit; i++) 
        {
            printf("%-5d | %-12s | %-15s | %10.2f\n", 
                   list[i].id, list[i].date, list[i].category, list[i].amount);
        }
        printf("... (Processed %d records | Capacity: %d) ...\n", count, capacity);
    }

    free(list);
    printf("\nMemory freed. Goodbye!\n");
    return 0;
}