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

void QuickSort(int *arr, int left_origin, int right_origin){
    if (left_origin >= right_origin) {
        return;
    }
    int left = left_origin, right = right_origin;
    int med = arr[(left + right)/2];
    while (left <= right) {
        while (left < right_origin + 1 && arr[left] < med) {
            left++;
        }
        while (right > -1 && arr[right] > med) {
            right--;
        }
        if (left <= right) {
            Swap(arr, left_origin + right_origin + 1, left, right);
            left++;
            right--;
        }
    }
    QuickSort(arr, left_origin, right);
    QuickSort(arr, left, right_origin);
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
    QuickSort(array, 0, n-1);
    for (int i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    free(array);
    return 0;
}
