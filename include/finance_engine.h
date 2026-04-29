#ifndef FINANCE_ENGINE_H
#define FINANCE_ENGINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
    int id;
    char date[11];
    char category[30];
    double amount;
    char description[50];
} Transaction;

int compareByAmount(const void *a, const void *b);
int compareByDate(const void *a, const void *b);
int compareByCategory(const void *a, const void *b);
void exportToCSV(Transaction *list, int count);
void generateCategorySummary(Transaction *list, int count);

#endif