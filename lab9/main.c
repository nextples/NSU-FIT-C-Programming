#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>


void CheckInput(int n, int m) {
    if (n < 0 || n > 5000) {
        printf("bad number of vertices");
        exit(EXIT_SUCCESS);
    }
    if (m < 0 || m > (n * (n - 1) / 2)) {
        printf("bad number of edges");
        exit(EXIT_SUCCESS);
    }
}


void FreeGraph(unsigned int **matrix, int numb_of_vert) {
    for (int j = 0; j < numb_of_vert; j++) {
        free(matrix[j]);
    }
    free(matrix);
}


int ReadGraph(unsigned int **graph, int numb_of_vert, int numb_of_edg) {
    long long length = 0;
    int start = 0;
    int end = 0;
    for (int i = 0; i < numb_of_edg; i ++) {
        if (scanf("%d %d %lld", &start, &end, &length) < 3) {
            printf("bad number of lines");
            return 0;
        }
        if (start < 1 || start > numb_of_vert || end < 1 || end > numb_of_vert) {
            printf("bad vertex");
            return 0;
        }
        if (length > INT_MAX || length < 0) {
            printf("bad length");
            return 0;
        }
        graph[start - 1][end - 1] = (unsigned int) length;
        graph[end - 1][start - 1] = (unsigned int) length;
    }
    return 1;
}


int MinDistance(long long *distance_from_s, bool *spt_set, int numb_of_vert) {
    long long min = LLONG_MAX;
    int min_index = 0;
    for (int i = 0; i < numb_of_vert; i++) {
        if (spt_set[i] == false && distance_from_s[i] < min) {
            min = distance_from_s[i];
            min_index = i;
        }
    }
    return min_index;
}


void PrintAnswer(long long *dist_from_s, int *parent, int numb_of_vert, int check_overflow, int start, int finish) {
    for (int i = 0; i < numb_of_vert; i++) {
        if (dist_from_s[i] <= INT_MAX) {
            printf("%lld ", dist_from_s[i]);
        }
        else {
            if (dist_from_s[i] != LLONG_MAX) {
                printf("INT_MAX+ ");
            }
            else {
                printf("oo ");
            }
        }
    }
    printf("\n");

    if (check_overflow == 1) {
        printf("overflow");
    }
    else {
        if (dist_from_s[finish] == LLONG_MAX) {
            printf("no path");
        }
        else {
            int current = finish;
            printf("%d ", finish + 1);
            while (current != start) {
                current = parent[current];
                printf("%d ", current + 1);
            }
        }
    }
}


void CreateSPT(unsigned int **graph, int numb_of_vert, int start, int finish) {
    bool *spt_set = (bool*) calloc(numb_of_vert, sizeof(bool));
    long long *dist_from_s = (long long*) calloc(numb_of_vert, sizeof(long long));
    for (int i = 0; i < numb_of_vert; i++) {
        dist_from_s[i] = LLONG_MAX;
    }
    int *parent = (int*) calloc(numb_of_vert, sizeof(int));

    parent[start] = -1;
    dist_from_s[start] = 0;

    int current = start;
    int check_overflow = 0;
    for (int i = 0; i < numb_of_vert; i++) {
        spt_set[current] = true;
        for (int j = 0; j < numb_of_vert; j++) {
            unsigned int curr_cell = graph[current][j];
            if (curr_cell && (dist_from_s[j] >= curr_cell + dist_from_s[current]) && spt_set[j] == false) {
                if (dist_from_s[j] > INT_MAX && dist_from_s[j] != LLONG_MAX && j == finish) {
                    check_overflow = 1;
                }
                dist_from_s[j] = dist_from_s[current] + curr_cell;
                parent[j] = current;
            }
        }
        current = MinDistance(dist_from_s, spt_set, numb_of_vert);
    }

    PrintAnswer(dist_from_s, parent, numb_of_vert, check_overflow, start, finish);
    free(spt_set);
    free(dist_from_s);
    free(parent);
}


int main() {
    int n = 0;
    int s = 0;
    int f = 0;
    int m = 0;
    if (scanf("%d %d %d %d", &n, &s, &f, &m) < 4) {
        printf("bad number of lines");
        exit(EXIT_SUCCESS);
    }

    if (n == 0) {
        printf("bad vertex");
        exit(EXIT_SUCCESS);
    }
    CheckInput(n, m);

    unsigned int **graph = (unsigned int**) calloc(n, sizeof(unsigned int*));
    for (int i = 0; i < n; i++) {
        graph[i] = (unsigned int*) calloc(n, sizeof(unsigned int));
    }

    if (!ReadGraph(graph, n, m)) {
        FreeGraph(graph, n);
        exit(EXIT_SUCCESS);
    }

    CreateSPT(graph, n, s - 1, f - 1);
    FreeGraph(graph, n);
    return EXIT_SUCCESS;
}
