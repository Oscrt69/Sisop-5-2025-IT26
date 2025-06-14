#include "std_lib.h"

int div(int a, int b) {
    int result, sign;
    if (b == 0) return 0;
    sign = ((a < 0) ^ (b < 0)) ? -1 : 1;
    result = 0;
    if (a < 0) { a = -a; sign *= -1; }
    if (b < 0) { b = -b; sign *= -1; }
    while (a >= b) {
        a -= b;
        result++;
    }

    return result * sign; 
}


int mod(int a, int b) {
    int c;
    if (b == 0) return 0;
    c = a - div(a, b) * b;
    return c;
}

bool strcmp(char *str1, char *str2) {
    while (*str1 && *str2) {
        if (*str1 != *str2) return false;
        str1++;
        str2++;
    }
    return *str1 == *str2;
}

void strcpy(char *dst, char *src) {
    while (*src) {
        *dst++ = *src++;
    }
    *dst = '\0';
}

void clear(byte *buf, unsigned int size) {
    unsigned int i;
    for (i = 0; i < size; i++) {
        buf[i] = 0;
    }
}

void atoi(char *str, int *num) {
    int i = 0; 
    int result = 0; 
    int sign = 1;
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
    while (str[i]== ' '){
        i++;
    }
    *num = result * sign;
}

void itoa(int num, char *str) {
    int i = 0, j, k;
    int isNegative = 0;
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (num < 0) {
        isNegative = 1;
        num = 0 - num;
    }

    while (num != 0) {
        str[i++] = (num % 10) + '0';
        num /= 10;
    }

    if (isNegative) str[i++] = '-';

    str[i] = '\0';

    for (j = 0, k = i - 1; j < k; j++, k--) {
        char *str;
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}      
