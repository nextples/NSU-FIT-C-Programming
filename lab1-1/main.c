#include <stdio.h>
#include <string.h>
#include <locale.h>
#define MAX_SIZE_OF_SAMPLE 17
#define MAX_SIZE_OF_TEXT 1024


int BinPow(int x, int p) {
    int res = 1;
    while (p > 0) {
        if (p & 1) {
            res *= x;
        }
        x *= x;
        p = p >> 1;
    }
    return res;
}


int GetHash(unsigned char *str, int len) {
    int hash = 0;
    for (int i = 0; i < len; ++i) {
        hash += (str[i] % 3) * (int)BinPow(3, i);
    }
    return hash;
}


int RecalculateHash(unsigned char* buffer, int index, int pattern_hash, int pattern_len) {
    pattern_hash -= (buffer[index - pattern_len] % 3);
    pattern_hash /= 3;
    pattern_hash += ((buffer[index] % 3) * (BinPow(3, pattern_len - 1)));
    return pattern_hash;
}


void CheckAndPrintIndexes(unsigned char *sample, unsigned char *buffer, int numb_iter, int index_buffer, int sample_len) {
    for (int i = 0; i < sample_len; i++) {
        printf(" %d", numb_iter * (MAX_SIZE_OF_TEXT - sample_len) + index_buffer + 1 + i);
        if (sample[i] != buffer[index_buffer + i]) {
            return;
        }
    }
}


void UpdateBuffer(unsigned char* buffer, int sample_len) {
    memcpy(buffer, buffer + MAX_SIZE_OF_TEXT - sample_len , sample_len);
    memset(buffer + sample_len , '\0', MAX_SIZE_OF_TEXT - sample_len);
}


void RabinKarpAlgorithm(unsigned char *sample, int sample_len, int sample_hash, FILE* f) {
    unsigned char buffer[MAX_SIZE_OF_TEXT] = {0};
    int numb_iteration = 0;
    if (fread(buffer, sizeof(unsigned char), sample_len, f) == 0) {
        return;
    }
    int pattern_hash = GetHash(buffer, sample_len);
    int i;
    do {
        unsigned long long len_str = fread(buffer + sample_len , sizeof(unsigned char), MAX_SIZE_OF_TEXT - sample_len, f) + sample_len;
        for (i = sample_len; i < MAX_SIZE_OF_TEXT; i++) {
            if (pattern_hash == sample_hash) {
                CheckAndPrintIndexes(sample, buffer, numb_iteration ,i - sample_len, sample_len);
            }
            pattern_hash = RecalculateHash(buffer, i, pattern_hash, sample_len);
            if ((int)len_str == sample_len) {
                return;
            }
            if (len_str == 0) {
                return;
            }
        }
        if (len_str == 0) {
            return;
        }
        UpdateBuffer(buffer, sample_len);
        numb_iteration++;
    }
    while (1);
}


int main() {
    FILE* f;
    f = fopen("in.txt", "r");
    setlocale(LC_ALL, "Rus");
    unsigned char symb = 0;
    int sample_len = 0;
    unsigned char sample[MAX_SIZE_OF_SAMPLE];
    while (symb != '\n') {
        symb = fgetc(f);
        sample[sample_len] = symb;
        sample_len++;
    }
    sample[sample_len - 1] = '\0';
    int sample_hash = GetHash(sample, sample_len);
    printf("%d ", sample_hash);
    RabinKarpAlgorithm(sample, sample_len-1, sample_hash, f);
    fclose(f);
    return 0;
}