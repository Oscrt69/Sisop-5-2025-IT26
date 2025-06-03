#include "kernel.h"
#include "std_lib.h"

// Deklarasi fungsi interrupt dari assembly kernel.asm
extern unsigned short _interrupt(unsigned char int_num, unsigned short ax, unsigned short bx, unsigned short cx, unsigned short dx);

// Wrapper fungsi interrupt agar sesuai prototype my_interrupt()
unsigned short my_interrupt(unsigned char int_num, unsigned short ax, unsigned short bx, unsigned short cx, unsigned short dx) {
    return _interrupt(int_num, ax, bx, cx, dx);
}

// Fungsi clear screen pakai interrupt BIOS scroll seluruh layar
void clearScreen() {
    my_interrupt(0x10, 0x0600, 0x0700, 0x0000, 0x184F);
}

// Fungsi printChar yang memanggil interrupt 0x10 BIOS
void printChar(char c) {
    my_interrupt(0x10, 0x0E00 | c, 0x0007, 0, 0);
}

// Fungsi printString sederhana
void printString(char* str) {
    while (*str) {
        printChar(*str++);
    }
}

// Fungsi string compare yang mengembalikan 1 jika sama, 0 jika beda
int mstrcmp(char* s1, char* s2) {
    while (*s1 && *s2 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (*s1 == '\0' && *s2 == '\0') ? 1 : 0;
}

// Fungsi clear buffer memory
void clear(void* ptr, int size) {
    char* p = (char*)ptr;
    for (int i = 0; i < size; i++) {
        p[i] = 0;
    }
}

// Deklarasi fungsi shell yang kamu punya di shell.c
extern void shell();

void main() {
    clearScreen();
    printString("Welcome to EorzeOS Kernel!\r\n");

    shell();

    // Loop infinite jika shell keluar
    while (1) {}
}
