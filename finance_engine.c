#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
    int id;
    char date[11];      // YYYY-MM-DD
    char category[30];  // 交易類別 (例如: 伙食, 交通)
    double amount;      // 交易金額
    char description[50];
} Transaction;

int compareTransactions(const void *a, const void *b) 
{
    Transaction *t1 = (Transaction *)a;
    Transaction *t2 = (Transaction *)b;
    
    // Desc Order
    if (t1->amount < t2->amount) return 1;
    if (t1->amount > t2->amount) return -1;
    return 0;
}

int main() 
{
    char filename[] = "data.csv";
    FILE *file = fopen(filename, "r");

    if (!file) 
    {
        printf("錯誤：找不到 %s 檔案。請確保檔案存在。\n", filename);
        return 1;
    }

    int capacity = 100;
    int count = 0;

    Transaction *list = (Transaction *)malloc(capacity * sizeof(Transaction));

    char line[150];
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) && count < capacity) 
    {
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

    printf("成功讀取 %d 筆交易紀錄。\n", count);

    // 執行高效排序
    qsort(list, count, sizeof(Transaction), compareTransactions);

    // 輸出分析結果
    printf("\n--- 高額交易分析報告 (依金額排序) ---\n");
    printf("%-5s | %-12s | %-10s | %-10s\n", "ID", "日期", "類別", "金額");
    printf("--------------------------------------------------\n");
    
    double total_expense = 0;
    for (int i = 0; i < count; i++) 
    {
        printf("%-5d | %-12s | %-10s | %10.2f\n", 
               list[i].id, list[i].date, list[i].category, list[i].amount);
        total_expense += list[i].amount;
    }

    printf("--------------------------------------------------\n");
    printf("總計支出： %.2f\n", total_expense);
    printf("平均每筆支出： %.2f\n", count > 0 ? total_expense / count : 0);

    free(list);
    printf("\n系統記憶體已釋放，程式結束。\n");

    return 0;
}