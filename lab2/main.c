#include <stdio.h>
#include <stdbool.h>
#define MAX 50


int len(char arr[]) {
    int i = 0;
    while (arr[i] != '\0') {
        ++i;
    }
    return i;
}


void DeleteLastSymb(char str[]) {
    int i = 0;
    while (str[i] != '\n') {
        i++;
    }
    str[i] = '\0';
}


void Swap(char arr[], int n, int i, int j) {
    if (i < 0 || j < 0 || i >= n || j >= n) {
        return;
    }
    char tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}


void ReverseArray(char arr[], int start) {
    for (int i = 0; i < (len(arr) - start)/2; ++i) {
        Swap(arr, len(arr), start + i, len(arr) - i - 1);
    }
}


void PrintNextPermutations(char string[], int permutationsCount) {
    for (int cnt = 0; cnt < permutationsCount; ++cnt) {
        for (int i = len(string) - 2; i >= 0; --i) {
            if (string[i] < string[i + 1]) {
                for (int j = len(string) - 1; j >= 0; --j) {
                    if (string[j] > string[i]) {
                        Swap(string, len(string), i, j);
                        ReverseArray(string, i + 1);
                        printf("%s\n", string);
                        break;
                    }
                }
                break;
            }
        }
    }
}


bool isInputValid(char string[]) {
    for (int i = 0; i < len(string); ++i) {
        if (string[i] < '0' || string[i] > '9'){
            return false;
        }
    }
    for (int i = 0; i < len(string); ++i) {
        for (int j = i+1; j < len(string); ++j) {
            if (string[i] == string[j]) {
                return false;
            }
        }
    }
    return true;
}


int main() {
    char buffer[MAX];
    int permutationsCount = 0;
    if (fgets(buffer, MAX, stdin) == NULL) {
        printf("bad input");
        return 0;
    }
    DeleteLastSymb(buffer);
    if (scanf("%d", &permutationsCount) != 1) {
        printf("bad input");
        return 0;
    }
    if (!isInputValid(buffer)) {
        printf("bad input");
        return 0;
    }
    PrintNextPermutations(buffer, permutationsCount);
    return 0;
}