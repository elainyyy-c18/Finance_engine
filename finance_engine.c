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

int main() 
{
    char filename[] = "data.csv";
    FILE *file = fopen(filename, "r");

    if (!file) 
    {
        printf("Error: Cannot find %s. Please ensure the file exists.\n", filename);
        return 1;
    }

    int capacity = 1000; // Start with a base capacity of 1000
    int count = 0;
    Transaction *list = (Transaction *)malloc(capacity * sizeof(Transaction));
    
    if (list == NULL) 
    {
        printf("Error: Initial memory allocation failed.\n");
        fclose(file);
        return 1;
    }

    char line[150];
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) 
    {
        if (count >= capacity) 
        {
            capacity *= 2; // Double capacity when full
            Transaction *temp = (Transaction *)realloc(list, capacity * sizeof(Transaction));
            
            if (temp == NULL) 
            {
                printf("Error: Memory reallocation failed at %d records.\n", count);
                free(list);
                fclose(file);
                return 1;
            }
            list = temp;
        }

        char *token;
        token = strtok(line, ",");
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
        printf(" [4] Exit Program\n");
        printf("-----------------------------------------\n");
        printf("Please enter your choice (1-4): ");
        
        if (scanf("%d", &choice) != 1) 
        {
            printf("\nInvalid input. Program exiting to protect memory.\n");
            break;
        }

        if (choice == 4) break;

        switch (choice) 
        {
            case 1: qsort(list, count, sizeof(Transaction), compareByAmount); break;
            case 2: qsort(list, count, sizeof(Transaction), compareByDate); break;
            case 3: qsort(list, count, sizeof(Transaction), compareByCategory); break;
            default: printf("\nInvalid choice. Please try again.\n"); continue;
        }

        printf("\n%-5s | %-12s | %-15s | %-10s\n", "ID", "Date", "Category", "Amount");
        printf("-------------------------------------------------------\n");
        int limit = (count > 15) ? 15 : count;
        for (int i = 0; i < limit; i++) {
            printf("%-5d | %-12s | %-15s | %10.2f\n", 
                   list[i].id, list[i].date, list[i].category, list[i].amount);
        }
        
        if (count > 15) 
        {
            printf("... (Total %d records loaded | Memory Capacity: %d) ...\n", count, capacity);
        }
    }

    free(list);
    printf("\nMemory freed. Thank you for using C-Engine!\n");
    return 0;
}