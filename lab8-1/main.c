#include <stdlib.h>
#include <stdio.h>
#include <limits.h>


void CheckInput(int n, int m) {
    if (n < 0 || n > 5000) {
        printf("bad number of vertices");
        exit(EXIT_SUCCESS);
    }
    if (m < 0 || m > ((n * (n + 1)) / 2)) {
        printf("bad number of edges");
        exit(EXIT_SUCCESS);
    }
    if (n == 0 || (n > 1 && m == 0)) {
        printf("no spanning tree");
        exit(EXIT_SUCCESS);
    }
    if (m == 0) {
        exit(EXIT_SUCCESS);
    }
}


int ReadGraph(int **matrix, int numb_of_vert, int numb_of_edg) {
    unsigned int length = 0;
    short start = 0;
    short end = 0;
    for (int i = 0; i < numb_of_edg; i ++) {
        if (scanf("%hd %hd %ud", &start, &end, &length) < 3) {
            printf("bad number of lines");
            return 0;
        }
        if (start < 1 || start > numb_of_vert || end < 1 || end > numb_of_vert) {
            printf("bad vertex");
            return 0;
        }
        if (length > INT_MAX) {
            printf("bad length");
            return 0;
        }
        matrix[start - 1][end - 1] = (int) length;
        matrix[end - 1][start - 1] = (int) length;
    }
    return 1;
}


void FreeGraph(int **matrix, int numb_of_vert) {
    for (int j = 0; j < numb_of_vert; j++) {
        free(matrix[j]);
    }
    free(matrix);
}


void InitArrayInfinite(unsigned int *arr, int numb_of_el, unsigned int val) {
    for (int i = 0; i < numb_of_el; i++) {
        arr[i] = val;
    }
}


void CheckConnectivity(int **matrix, char *used, int numb_of_vert, int i) {
    for (int j = 0; j < numb_of_vert; j++) {
        if (used[j] == 0 && matrix[i][j] != 0) {
            used[j] = 1;
            CheckConnectivity(matrix, used, numb_of_vert, j);
        }
    }
}


int FindMinKey(unsigned int key[], char mstSet[], int numb_of_vert) {
    unsigned int min_val = UINT_MAX;
    int min_val_ind = 0;
    for (int i = 0; i < numb_of_vert; i++) {
        if (mstSet[i] == 0 && key[i] < min_val) {
            min_val = key[i];
            min_val_ind = i;
        }
    }
    return min_val_ind;
}


void PrintMST(int parent[], int numb_of_vert) {
    for (int i = 1; i < numb_of_vert; i++) {
        printf("%d %d \n", parent[i] + 1, i + 1);
    }
}


void CreateMST(int **graph, int numb_of_vert) {
    char *included_vert = (char*) calloc(numb_of_vert, sizeof(char));
    unsigned int *key = (unsigned int*) calloc(numb_of_vert, sizeof(unsigned int));
    int *parent = (int*) calloc(numb_of_vert, sizeof(int));
    InitArrayInfinite(key, numb_of_vert, UINT_MAX);

    key[0] = 0;
    parent[0] = -1;

    for (int i = 0; i < numb_of_vert - 1; i++) {
        int index_key = FindMinKey(key, included_vert, numb_of_vert);
        included_vert[index_key] = 1;

        //Обновить ключ для всех соседних с u вершинами
        //Для каждой смежной вершины v, если вес ребра u-v меньше предыдущего ключевого значения v, обновить ключевое значение как вес u-v.
        for (int j = 0; j < numb_of_vert; j++) {
            if (graph[index_key][j] && included_vert[j] == 0 && (unsigned int)graph[index_key][j] < key[j]) {
                parent[j] = index_key;
                key[j] = graph[index_key][j];
            }
        }
    }
    PrintMST(parent, numb_of_vert);
    free(included_vert);
    free(key);
    free(parent);
}


int main() {
    int n = 0;
    int m = 0;
    if (!scanf("%d", &n)) {
        printf("bad number of lines");
        exit(EXIT_SUCCESS);
    }
    if (!scanf("%d", &m)) {
        printf("bad number of lines");
        exit(EXIT_SUCCESS);
    }
    CheckInput(n, m);

    int **matrix = (int**) calloc(n, sizeof(int*));
    for (int i = 0; i < n; i++) {
        matrix[i] = (int*) calloc(n, sizeof(int));
    }

    if (!ReadGraph(matrix, n, m)) {
        FreeGraph(matrix, n);
        exit(EXIT_SUCCESS);
    }

    char *used = (char*) calloc(n, sizeof(char));
    CheckConnectivity(matrix, used, n, 0);
    for (int i = 0; i < n; i++) {
        if (used[i] == 0) {
            printf("no spanning tree");
            free(used);
            FreeGraph(matrix, n);
            exit(EXIT_SUCCESS);
        }
    }

    CreateMST(matrix, n);
    free(used);
    FreeGraph(matrix, n);
}