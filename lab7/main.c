#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MaxSize 2000
#define Byte 8
#define LenghtMatrix 250

typedef struct Stack {
    int array[MaxSize];
    int stack_len;
} Stack;


typedef struct Graph {
    char matrix[MaxSize][LenghtMatrix];
    Stack* stack;
    char used[MaxSize];
} Graph;


Graph* CreateGraph(Graph* graph) {
    graph = calloc(1, sizeof(Graph));
    graph->stack = calloc(1, sizeof(Stack));
    return graph;
}


void CheckInput(int n, int m) {
    if (n < 0 || n > MaxSize) {
        printf("bad number of vertices");
        exit(EXIT_SUCCESS);
    }
    if (m < 0 || m > ((n * (n + 1)) / 2)) {
        printf("bad number of edges");
        exit(EXIT_SUCCESS);
    }
}


void CheckVertex(int v, int n) {
    if (v < 1 || v > n) {
        printf("bad vertex");
        exit(EXIT_SUCCESS);
    }
}


void AddEdge(char* matrix, int a, int b) {
    int block_number = b / Byte;
    int bit_number = b % Byte;
    matrix[a * LenghtMatrix + block_number] |= (1 << (Byte-1 - bit_number));
}


bool IsConnected(char* matrix, int string_number, int block_number, int bit_number) {
    if (matrix[string_number * LenghtMatrix + block_number] & (1 << (Byte-1 - bit_number))){
        return true;
    }
    return false;
}


void Push(Stack* stack, int vertex) {
    stack->array[stack->stack_len] = vertex;
    stack->stack_len++;
}


void Pop(Stack* stack) {

    printf("%d ", stack->array[stack->stack_len - 1]);
    stack->stack_len--;
}


void DestructGraph(Graph* graph) {
    free(graph->stack);
    free(graph);
}


void DFSTopSort(Graph* graph, int start, char* arr) {
    graph->used[start] = 1;
    arr[start] = 1;
    for (int i = 0; i < LenghtMatrix; i++) {
        for (int j = 0; j < Byte; j++) {
            if (IsConnected(*graph->matrix, start, i, j) == true) {
                int new_start = i * 8 + j;
                if (graph->used[new_start] == 0 && arr[new_start] != 1) {
                    DFSTopSort(graph, new_start, arr);
                }
                else if (arr[new_start] == 1) {
                    printf("impossible to sort");
                    DestructGraph(graph);
                    exit(EXIT_SUCCESS);
                }
            }
        }
    }
    Push(graph->stack, start + 1);
    arr[start]++;
}


int main() {
    int n = 0;
    int m = 0;
    if (scanf("%d", &n) == -1) {
        printf("bad number of lines");
        exit(EXIT_SUCCESS);
    }
    if (scanf("%d", &m) == -1) {
        printf("bad number of lines");
        exit(EXIT_SUCCESS);
    }
    CheckInput(n, m);
    Graph *graph = NULL;
    graph = CreateGraph(graph);
    int from, to;
    for (int i = 0; i < m; i++) {
        if (scanf("%d %d", &from, &to) == -1 && m != 0) {
            printf("bad number of lines");
            DestructGraph(graph);
            exit(EXIT_SUCCESS);
        }
        CheckVertex(from, n);
        CheckVertex(to, n);
        AddEdge(*graph->matrix, from - 1, to - 1);
    }

    char loop_marker[MaxSize];
    for (int i = 0; i < n; i++) {
        if (!graph->used[i]){
            memset(loop_marker, 0, MaxSize);
            DFSTopSort(graph, i, loop_marker);
        }
    }
    while (graph->stack->stack_len != 0) {
        Pop(graph->stack);
    }
    DestructGraph(graph);
    return EXIT_SUCCESS;
}
