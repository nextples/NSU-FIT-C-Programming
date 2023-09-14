#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

double TransmitToDecimal(char a[], int base) {
    double res = 0;
    char before[14] = {0}, after[15] = {0};
    int dot = 0;
    for (size_t i = 0; i < strlen(a); ++i) {   // Заполнение массива цифрами до точки (строка целой части)
        if (a[i] != '.') {
            before[i] = a[i];
        }
        else {
            dot = i;
            break;
        }
    }
    after[0] = '0';
    after[1] = '.';
    if (dot != 0) {
        for (size_t i = dot + 1; i < strlen(a); ++i) {   // Заполнение массива цифрами после точки (строка дробной части)
            after[i - dot + 1] = a[i];
        }
    }
    for (size_t i = 0; i < strlen(before); ++i) {  // Вычисление целой части
        if (before[i] >= '0' && before[i] <= '9') {
            res += (before[i] - '0') * pow(base, strlen(before) - i - 1);
        }
        if (before[i] >= 'a' && before[i] <= 'f') {
            res += (before[i] - 'a' + 10) * pow(base, strlen(before) - i - 1);
        }
    }
    for (size_t i = 2; i < strlen(after); ++i) {   // Вычисление дробной части
        if (after[i] >= '0' && after[i] <= '9') {
            res += (after[i] - '0') * 1/pow(base, i - 1);
        }
        if (after[i] >= 'a' && after[i] <= 'f') {
            res += (after[i] - 'a' + 10) * 1/pow(base, i - 1);
        }
    }
    return res;
}

#define SIZE_REMAINDERS 150
#define SIZE_INTEGERS 120
#define SIZE_DROB 130
#define SIZE_ANSWER 10000


char* TransmitFromDecimal(double res, int base) {
    long long before = floor(res);
    double after = res - before;
    long long before_remember = before;
    double isdot = after;
    int remainders[SIZE_REMAINDERS] = {0};
    int integers[SIZE_INTEGERS] = {0};

    for (size_t i = 0; i < SIZE_INTEGERS; ++i) {
        integers[i] = -1;
    }
    for (int i = 0; i < SIZE_REMAINDERS; ++i) {
        remainders[i] = -1;
    }
    int index = 0;
    while (before > 0) {     // Заполнение массива для перевода целой части остатками от деления
        remainders[index] = before % base;
        before /= base;
        index++;
    }
    if (before_remember == 0) {
        remainders[0] = 0;
    }
    if (after != 0) {
        for (int i = 0; i < SIZE_INTEGERS; ++i) {       // Заполнение массива для перевода дробной части целыми частями от умножения
            integers[i] = floor(after * base);
            after = after * base - floor(after * base);
        }
    }
    char *answer = malloc(SIZE_ANSWER);
    for (int i = 0; i < SIZE_ANSWER; i++) {
        answer[i] = 0;
    }
    for (int i = 0; i < SIZE_REMAINDERS; ++i) {      // Составление целой части
        if (remainders[i] != -1) {
            if (remainders[i] >= 0 && remainders[i] <= 9) {
                answer[i] = (char) (remainders[i] + '0');
            }
            if (remainders[i] >= 10 && remainders[i] <= 15) {
                answer[i] = (char) (remainders[i] + 'a' - 10);
            }
        }
    }

    for (size_t i = 0; i < strlen(answer)/2; ++i) {     // Переворачивание массива целой части
        char tmp = answer[i];
        answer[i] = answer[strlen(answer) - i - 1];
        answer[strlen(answer) - i - 1] = tmp;
    }
    char drob[SIZE_DROB] = {0};
    for (int i = 0; i < SIZE_INTEGERS; ++i) {        // Составление массива дробной части
        if (integers[i] >= 0 && integers[i] <= 9) {
            drob[i] = (char) (integers[i] + '0');
        }
        if (integers[i] >= 10 && integers[i] <= 15) {
            drob[i] = (char) (integers[i] + 'a' - 10);
        }
    }
    int cnt = (int)strlen(answer);
    for (size_t i = 0; i < strlen(drob); ++i) {
        answer[strlen(answer)] = drob[i];
    }
    if (isdot != 0) {
        for (size_t j = strlen(answer); j > cnt - 1; j--) {
            answer[j + 1] = answer[j];
        }
        answer[cnt] = '.';
    }
    return answer;
}

bool ConditionOfInputBase(int a1, int a2) {
    if (a1 < 2 || a1 > 16 || a2 < 2 || a2 > 16) {
        return false;
    }
    return true;
}

bool ConditionOfInputSymb(char a[], int base) {
    int cnt = 0;
    for (size_t i = 0; i < strlen(a); ++i) {
        if (a[i] == '.') {
            cnt += 1;
        }
        if (cnt > 1) {
            return false;
        }
        if ((a[i] >= '0' && a[i] <= '9' && a[i] - '0' + 1 > base) ||
            (a[i] >= 'a' && a[i] <= 'f' && a[i] - 'a' + 1 + 10 > base) ||
            (((a[i] < '0' - 1) || (a[i] > '9' && a[i] < 'a') || (a[i] > 'f')) && a[i] != '.') ||
            (a[0] == '.') ||
            (a[strlen(a) - 1] == '.')
                ) {
            return false;
        }
    }
    return true;
}

void LowerLetter(char a[]) {
    for (size_t i = 0; i < strlen(a); i++) {
        if (a[i] >= 'A' && a[i] <= 'Z') {
            a[i] = a[i] + 32;
        }
    }
}

#define SIZE_NUMB 14

int main(){
    int b1 = 0, b2 = 0;
    if ((scanf("%d %d", &b1, &b2)) != 2) {
        printf("bad input");
    }
    char numb[SIZE_NUMB] = {0};
    if (scanf("%13s", numb) == 0) {
        printf("bad input");
    }
    LowerLetter(numb);
    if (ConditionOfInputBase(b1, b2) && ConditionOfInputSymb(numb, b1)) {
        double part1 = TransmitToDecimal(numb, b1);
        char *pointer = TransmitFromDecimal(part1, b2);
        printf("%s", pointer);
        free(pointer);
    }
    else {
        printf("bad input");
    }
    return 0;
}
