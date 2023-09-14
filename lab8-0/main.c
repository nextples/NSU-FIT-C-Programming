#include <stdlib.h>
#include <stdio.h>
#include <limits.h>


typedef struct {
    int start;
    int end;
    int length;
} Edge;


typedef struct {
    Edge *list;
    int len;
} List_Edge;


int Comparator(const void *a, const void *b) {
    return ((Edge*) a)->length - ((Edge*) b)->length;
}


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


int FindElement(int *arr, int numb) {
    return arr[numb];
}


void ApplyUnion(int *arr, int n, int cno1, int cno2) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == cno2) {
            arr[i] = cno1;
        }
    }
}


int IsGraphRelated (Edge *edges, int edge_num, int vert_num){
    int *visited_vert = (int*) calloc(vert_num, sizeof(int));
    for (int i = 0; i < edge_num; i++) {
        visited_vert[edges[i].start - 1] = 1;
        visited_vert[edges[i].end - 1] = 1;
    }
    for (int i = 0; i < vert_num; i++) {
        if (visited_vert[i] == 0) {
            printf("no spanning tree");
            free(visited_vert);
            return 0;
        }
    }
    free(visited_vert);
    return 1;
}


void CreateDSU(int *set, Edge *list_edges, List_Edge* answ, int numb_of_vert, int numb_of_edg) {
    for (int i = 0; i < numb_of_edg; i++) {
        int cno1 = FindElement(set, list_edges[i].start - 1);
        int cno2 = FindElement(set, list_edges[i].end - 1);
        if (cno1 != cno2) {
            answ->list[answ->len] = list_edges[i];
            answ->len++;
            ApplyUnion(set, numb_of_vert, cno1, cno2);
        }
    }
}


void ReadEdges(Edge* list_edges, int numb_of_vert, int numb_of_edg) {
    long long length = 0;
    for (int i = 0; i < numb_of_edg; i++) {
        if (scanf("%d %d %lld", &list_edges[i].start, &list_edges[i].end, &length) == -1) {
            printf("bad number of lines");
            exit(EXIT_SUCCESS);
        }

        if (list_edges[i].start < 1 || list_edges[i].start > numb_of_vert || list_edges[i].end < 1 || list_edges[i].end > numb_of_vert) {
            printf("bad vertex");
            exit(EXIT_SUCCESS);
        }

        if (length > INT_MAX || length < 0) {
            printf("bad length");
            exit(EXIT_SUCCESS);
        }
        list_edges[i].length = (int)length;
    }
}


void InitArr(int *arr, int numb_of_el) {
    for (int i = 0; i < numb_of_el; i++) {
        arr[i] = i;
    }
}


void PrintAnsw(List_Edge arr) {
    for (int i = 0; i < arr.len; i++) {
        printf("%d %d\n", arr.list[i].start, arr.list[i].end);
    }
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

    Edge *list_edges = (Edge*)calloc(m, sizeof(Edge));

    ReadEdges(list_edges, n, m);
    qsort(list_edges, m, sizeof(Edge), Comparator);

    List_Edge answ = {0};
    answ.list = (Edge*)calloc(n, sizeof(Edge));

    int *set = (int*) calloc(n, sizeof(int));
    InitArr(set, n);

    if (!IsGraphRelated(list_edges, m, n)) {
        free(list_edges);
        free(answ.list);
        free(set);
        exit(EXIT_SUCCESS);
    }
    CreateDSU(set, list_edges, &answ, n, m);
    PrintAnsw(answ);

    free(list_edges);
    free(answ.list);
    free(set);
    return EXIT_SUCCESS;
}
