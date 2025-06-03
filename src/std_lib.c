#include "std_lib.h"
#include "kernel.h"

void printChar(char c) {
    my_interrupt(0x10, 0x0E00 | c, 0x0007, 0, 0);
}

void printString(char* str) {
    while (*str) {
        printChar(*str++);
    }
}

void clearScreen() {
    my_interrupt(0x10, 0x0600, 0x0700, 0x0000, 0x184F);
}

int mstrcmp(char* s1, char* s2) {
    while (*s1 && *s2 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (*s1 == '\0' && *s2 == '\0') ? 1 : 0;
}

void clear(void* ptr, int size) {
    char* p = (char*)ptr;
    for (int i = 0; i < size; i++) {
        p[i] = 0;
    }
}
