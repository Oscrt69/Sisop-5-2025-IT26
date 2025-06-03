#include "std_lib.h"

// Fungsi pembagian bulat (integer division)
int div(int a, int b) {
    if (b == 0) return 0; // Hindari pembagian nol (bisa disesuaikan error handling)
    int sign = 1;
    if (a < 0) { sign = -sign; a = -a; }
    if (b < 0) { sign = -sign; b = -b; }
    int q = 0;
    while (a >= b) {
        a = a - b;
        q++;
    }
    return sign * q;
}

// Fungsi modulus (sisa bagi)
int mod(int a, int b) {
    if (b == 0) return 0; // Hindari modulus nol
    int sign = 1;
    if (a < 0) sign = -1;
    if (b < 0) b = -b;
    while (a >= b) {
        a = a - b;
    }
    return sign * a;
}

// Bandingkan dua string, return true jika sama, false jika beda
bool strcmp(char *str1, char *str2) {
    int i = 0;
    while (str1[i] && str2[i]) {
        if (str1[i] != str2[i]) return false;
        i++;
    }
    return str1[i] == str2[i];
}

// Salin string src ke dst
void strcpy(char *dst, char *src) {
    int i = 0;
    while ((dst[i] = src[i]) != 0) i++;
}

// Isi buf dengan 0 sebanyak size bytes
void clear(byte *buf, unsigned int size) {
    for (unsigned int i = 0; i < size; i++) {
        buf[i] = 0;
    }
}

// Konversi string ke integer, simpan hasil di *num
// Meng-handle angka negatif dan spasi awal
void atoi(char *str, int *num) {
    int i = 0;
    int sign = 1;
    int result = 0;

    // Lewati spasi
    while (str[i] == ' ') i++;

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

    *num = sign * result;
}

// Konversi integer ke string (basis 10)
void itoa(int num, char *str) {
    char buf[12]; // buffer cukup untuk int
    int i = 0, j;
    int sign = 1;

    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    if (num < 0) {
        sign = -1;
        num = -num;
    }

    while (num != 0) {
        buf[i++] = (num % 10) + '0';
        num = num / 10;
    }

    if (sign == -1) {
        buf[i++] = '-';
    }

    // Balik string ke output
    for (j = 0; j < i; j++) {
        str[j] = buf[i - j - 1];
    }
    str[i] = '\0';
}
