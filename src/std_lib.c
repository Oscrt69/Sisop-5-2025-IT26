#include "std_lib.h"

// Hitung panjang string
int strlen(const char *str) {
    int len = 0;
    while (str[len]) len++;
    return len;
}

// Bandingkan dua string, return 0 jika sama
int strcmp(const char *a, const char *b) {
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return (unsigned char)a[i] - (unsigned char)b[i];
        i++;
    }
    return (unsigned char)a[i] - (unsigned char)b[i];
}

// Salin string src ke dest
void strcpy(char *dest, const char *src) {
    int i = 0;
    while ((dest[i] = src[i]) != 0) i++;
}

// Salin n karakter dari src ke dest
void strncpy(char *dest, const char *src, int n) {
    int i = 0;
    for (; i < n && src[i]; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
}

// Konversi string ke integer, support negatif
int atoi(const char *str) {
    int i = 0, sign = 1, result = 0;

    // Lewati spasi
    while (str[i] == ' ') i++;

    // Cek tanda minus
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return sign * result;
}

// Konversi integer ke string basis 10
void itoa(int value, char *str) {
    char buf[12]; // cukup untuk -2147483648
    int i = 0, j, sign;

    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    sign = value < 0 ? -1 : 1;
    if (sign == -1) value = -value;

    while (value != 0) {
        buf[i++] = (value % 10) + '0';
        value /= 10;
    }

    if (sign == -1) buf[i++] = '-';

    // Balik string
    for (j = 0; j < i; j++) {
        str[j] = buf[i - j - 1];
    }
    str[i] = '\0';
}

// Random sederhana: Linear Congruential Generator
static unsigned int seed = 1;

void srand(unsigned int s) {
    seed = s;
}

unsigned int rand(void) {
    seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return seed;
}
