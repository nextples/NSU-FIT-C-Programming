#include <stdlib.h>
#include <stdio.h>


typedef struct item {
    int weight;
    int price;
} item;


int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

int cnt = 0;
void IsItemTaken(int **matrix, int n, int w, item *taken_items, item *list_items) {
    if (matrix[n][w] == 0) {
        return;
    }
    if (matrix[n-1][w] == matrix[n][w]) {
        IsItemTaken(matrix, n-1, w, taken_items, list_items);
    }
    else {
        cnt += 1;
        IsItemTaken(matrix, n-1, w - list_items[n-1].weight, taken_items, list_items);
        taken_items[n-1] = list_items[n-1];
    }
}


void FillMatrix(int **matrix, int str, int w, int weight, int price) {
    for (int col = 0; col < w; col++) {
        if (col - weight >= 0 && str - 1 >= 0) {
            matrix[str][col] = max(matrix[str - 1][col], price + matrix[str - 1][col - weight]);
        }
        else {
            matrix[str][col] = matrix[str - 1][col];
        }
    }
}

void FreeMemory(int n, int **arr1, item *arr2, item *arr3) {
    for (int j = 0; j < n; j++) {
        free(arr1[j]);
    }
    free(arr1);
    free(arr2);
    free(arr3);
}



int main() {
    int n = 0;
    int w = 0;
    if (!scanf("%d", &n)) {
        return EXIT_SUCCESS;
    }
    if (!scanf("%d", &w)) {
        return EXIT_SUCCESS;
    }
    n++;
    w++;
    int **matrix = (int**)calloc(n, sizeof(int*));
    for (int i = 0; i < n; i++) {
        matrix[i] = (int*)calloc(w, sizeof(int));
    }
    item *list_items = (item*)calloc(n, sizeof(item));
    item *taken_items = calloc(n, sizeof(item));

    for (int i = 0; i < n - 1; i++) {
        if (!scanf("%d%d", &(list_items[i]).weight, &list_items[i].price)) {
            FreeMemory(n, matrix, list_items, taken_items);
            return EXIT_SUCCESS;
        }
        FillMatrix(matrix, i+1, w, list_items[i].weight, list_items[i].price);
    }

    printf("%d\n", matrix[n-1][w-1]);
    IsItemTaken(matrix, n-1, w-1, taken_items, list_items);
    for (int i = 0; i < n; i++) {
        if (taken_items[i].weight != 0 && taken_items[i].price != 0) {
            printf("%d %d\n", taken_items[i].weight, taken_items[i].price);
        }
    }
    FreeMemory(n, matrix, list_items, taken_items);
    return EXIT_SUCCESS;
}
