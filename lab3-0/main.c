#include <stdio.h>
#include <stdlib.h>

void Swap(int arr[], int n, int i, int j) {
    if (i >= n || j >= n) {
        return;
    }
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

void Sift(int arr[], int n, int root) {
    if (root >= n) {
        return;
    }
    int ind_left = 2*root + 1, ind_right = 2*root + 2;
    int ind_max = root;
    if (ind_left < n && arr[ind_left] > arr[ind_max])
        ind_max = ind_left;
    if (ind_right < n && arr[ind_right] > arr[ind_max])
        ind_max = 2*root+2;
    if (ind_max != root) {
        Swap(arr, n, ind_max, root);
        Sift(arr, n, ind_max);
    }
}


void HeapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        Sift(arr, n, i);
    }
    for (int i = n - 1; i >= 0; --i) {
        Swap(arr, n, 0, i);
        Sift(arr, i, 0);
    }
}



int main() {
    int n = 0;
    if (scanf("%d", &n) != 1) {
        return 0;
    }
    int *array = malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &array[i]) != 1) {
            return 0;
        }
    }
    HeapSort(array, n);
    for (int i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    free (array);
    return 0;
}
