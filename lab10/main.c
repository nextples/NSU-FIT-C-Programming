#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_N 100000

typedef struct Point {
    int x, y;
}Point;

double Rotate(Point A, Point B, Point C) {
    return (((double)B.x - A.x) * ((double)C.y - B.y) - ((double)B.y - A.y) * ((double)C.x - B.x)); //  > 0 - слева (точка С)
                                                                                                    //  < 0 - справа (точка С)
                                                                                                    //  = 0 - на одной прямой
}

void Swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void FindBottomPoint(Point points[], int listOfPoints[], int n) {
    for (int i = 1; i < n; i++) {
        if ((points[listOfPoints[i]].x < points[listOfPoints[0]].x)
            || (points[listOfPoints[i]].x == points[listOfPoints[0]].x && points[listOfPoints[i]].y < points[listOfPoints[0]].y)) {

            Swap(listOfPoints + i, listOfPoints);
        }
    }
    listOfPoints[n] = listOfPoints[0];
}
// сортировка по "правизне"
void SortPoints(int *listOfPoints, Point *points, int first, int last) {
    if (first >= last) {
        return;
    }
    int left = first;
    int right = last;
    Point lowest = points[listOfPoints[0]];
    Point middle = points[listOfPoints[(left + right) / 2]];

    do {
        while (1) {
            double r = Rotate(lowest, middle, points[listOfPoints[left]]);
            if (r >= 0) {
                break;
            }
            left++;
        }
        while (Rotate(lowest, middle, points[listOfPoints[right]]) > 0) {
            right--;
        }

        if (left <= right) {
            Swap(listOfPoints + left, listOfPoints + right);
            left++;
            right--;
        }
    } while (left <= right);
    SortPoints(listOfPoints, points, first, right);
    SortPoints(listOfPoints, points, left, last);
}

void PrintPoints(Point points[], int size, const int convexHull[]) {
    for (int i = 0; i < size; i++) {
        printf("%d %d\n", points[convexHull[i]].x, points[convexHull[i]].y);
    }
}

void GeneralCase(Point *points, const int *listOfPoints, int *convexHull, int n, int *size) {
    convexHull[0] = listOfPoints[0];
    convexHull[1] = listOfPoints[1];
    int j = 2;
    double rot;
    Point A, B, C;
    for (int i = 2; i <= n; i++) {
        while (j > 1) {
            A = points[convexHull[j - 2]];
            B = points[convexHull[j - 1]];
            C = points[listOfPoints[i]];
            rot = Rotate(A, B, C);
            if (rot > 0) {
                break;
            }
            j--;
        }
        convexHull[j] = listOfPoints[i];
        j++;
    }
    if (j > 2) {
        *size = j - 1;
    }
    else {
        *size = j;
    }
}

void OneLineCase(Point *points, const int *listOfPoints, int *convexHull, int n, int *size) {
    if (points[listOfPoints[0]].x == points[listOfPoints[n - 1]].x) {
        double maxY = (double)points[listOfPoints[0]].y;
        int indMaxY = 0;
        for (int i = 1; i < n; i++) {
            if (points[listOfPoints[i]].y > maxY)
            {
                maxY = points[listOfPoints[i]].y;
                indMaxY = i;
            }
        }
        convexHull[0] = listOfPoints[0];
        *size = 2;
        convexHull[1] = listOfPoints[indMaxY];
    }
    else {
        double maxX = (double)points[listOfPoints[0]].x;
        int IndMaxX = 0;
        for (int i = 1; i < n; i++) {
            if (points[listOfPoints[i]].x > maxX) {
                maxX = points[listOfPoints[i]].x;
                IndMaxX = i;
            }
        }
        convexHull[0] = listOfPoints[0];
        convexHull[1] = listOfPoints[IndMaxX];
        *size = 2;
    }
}

void GrahamScan(Point *points, int *listOfPoints, int *convexHull, int n, int *size) {
    if (n == 0) {
        return;
    }
    bool oneLineFlag = true;
    for (int i = 0; i < n; i++) { // есть ли точка в стороне от первого отрезка МВО?
        convexHull[0] = listOfPoints[0];
        convexHull[1] = listOfPoints[1];
        int j = 2;
        if (Rotate(points[convexHull[j - 2]], points[convexHull[j - 1]], points[listOfPoints[i]]) != 0) {
            oneLineFlag = false;
            break;
        }
    }
    if (oneLineFlag == 1) {
        OneLineCase(points, listOfPoints, convexHull, n, size);
    }
    else {
        GeneralCase(points, listOfPoints, convexHull, n, size);
    }
}

int main() {
    int n;
    if (!scanf("%d", &n)) {
        return EXIT_SUCCESS;
    }
    if (n < 0 || n > MAX_N) {
        printf("bad number of points");
        return EXIT_SUCCESS;
    }

    Point *points = malloc(n * sizeof(Point));
    for (int i = 0; i < n; i++) {
        if (scanf("%d %d", &points[i].x, &points[i].y) != 2)
        {
            printf("bad number of lines");
            free(points);
            return EXIT_SUCCESS;
        }
    }

    if (n == 1) {
        printf("%d %d", points[0].x, points[0].y);
        free(points);
        return EXIT_SUCCESS;
    }

    int *listOfPoints = malloc((n + 1) * sizeof(int));
    if (listOfPoints == NULL) {
        free(points);
        return EXIT_SUCCESS;
    }

    for (int i = 0; i < n; i++) {
        listOfPoints[i] = i;
    }

    FindBottomPoint(points, listOfPoints, n);
    SortPoints(listOfPoints, points, 1, n - 1);

    int *convexHull = malloc((n + 1) * sizeof(int));
    if (convexHull == NULL) {
        free(points);
        free(listOfPoints);
        return EXIT_SUCCESS;
    }

    int size = 0;
    GrahamScan(points, listOfPoints, convexHull, n, &size);
    PrintPoints(points, size, convexHull);

    free(points);
    free(listOfPoints);
    free(convexHull);
    return 0;
}