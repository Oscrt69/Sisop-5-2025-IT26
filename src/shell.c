#include "std_lib.h"
#include "kernel.h"

// Fungsi interrupt dari kernel.asm
extern unsigned short my_interrupt(unsigned char int_num, unsigned short ax, unsigned short bx, unsigned short cx, unsigned short dx);

#define MAX_INPUT 128
#define MAX_NAME 32

// Variabel global
char userName[MAX_NAME] = "user";
char grandCompanyTitle[16] = "";  // @Storm, @Serpent, @Flame, atau kosong
byte textColor = 0x07;             // warna default putih (grey)

// Daftar random yogurt replies
char* yogurtReplies[] = {
    "yo",
    "ts unami gng </3",
    "sygau",
    "gurt",
    "yosh",
    "hakuna matata"
};
int yogurtCount = sizeof(yogurtReplies) / sizeof(char*);

// Fungsi bantu print char dan string
void printChar(char c) {
    my_interrupt(0x10, 0x0E00 | c, textColor, 0, 0);
}

void printString(char* s) {
    while (*s) {
        printChar(*s++);
    }
}

// Clear layar dengan interrupt BIOS scroll
void clearScreen() {
    my_interrupt(0x10, 0x0600, 0x0700, 0x0000, 0x184F);
}

// Set warna teks terminal (AX=0x0B00 + color)
void setTextColor(byte color) {
    textColor = color;
    my_interrupt(0x10, 0x0B00 | color, 0, 0, 0);
}

// Membaca input string dengan echo, maksimal MAX_INPUT-1
void readString(char* buf) {
    int i = 0;
    char c;
    while (i < MAX_INPUT - 1) {
        unsigned short key = my_interrupt(0x16, 0, 0, 0, 0);
        c = key & 0xFF;

        if (c == '\r') {
            buf[i] = 0;
            printString("\r\n");
            return;
        } else if (c == 0x08) { // backspace
            if (i > 0) {
                i--;
                printString("\b \b");
            }
        } else if (c >= 32 && c <= 126) {
            buf[i++] = c;
            printChar(c);
        }
    }
    buf[i] = 0;
}

// Fungsi strcmp custom (return 1 jika sama)
int mstrcmp(char* s1, char* s2) {
    while (*s1 && *s2) {
        if (*s1 != *s2) return 0;
        s1++; s2++;
    }
    return (*s1 == 0 && *s2 == 0);
}

// Fungsi untuk parsing input menjadi kata pertama dan sisa (simple)
void splitCommand(char* input, char* cmd, char* arg) {
    int i = 0;
    // Copy cmd sampai spasi atau null
    while (input[i] && input[i] != ' ' && i < MAX_INPUT) {
        cmd[i] = input[i];
        i++;
    }
    cmd[i] = 0;
    // Lewatkan spasi
    int j = 0;
    while (input[i] == ' ') i++;
    // Copy sisa ke arg
    while (input[i] && j < MAX_INPUT - 1) {
        arg[j++] = input[i++];
    }
    arg[j] = 0;
}

// Parsing dua arg integer untuk kalkulator
int parseTwoInts(char* arg, int* x, int* y) {
    // Contoh input: "4 2"
    int i = 0;
    char num1[32] = {0};
    char num2[32] = {0};
    int idx = 0;

    // Ambil angka pertama
    while (arg[i] && arg[i] != ' ') {
        if (idx >= 31) return 0;
        num1[idx++] = arg[i++];
    }
    num1[idx] = 0;
    while (arg[i] == ' ') i++;
    idx = 0;
    // Ambil angka kedua
    while (arg[i]) {
        if (idx >= 31) return 0;
        num2[idx++] = arg[i++];
    }
    num2[idx] = 0;

    if (!num1[0] || !num2[0]) return 0;
    // Konversi ke int
    *x = atoi(num1);
    *y = atoi(num2);
    return 1;
}

// Fungsi utama shell
void shell() {
    char input[MAX_INPUT];
    char cmd[32], arg[MAX_INPUT];
    int running = 1;

    printString("EorzeOS Shell v1.0\r\n");
    printString("Ketik 'help' untuk daftar perintah.\r\n");

    while (running) {
        // Print prompt: userName + grandCompanyTitle + "> "
        printString(userName);
        if (grandCompanyTitle[0]) {
            printChar('@');
            printString(grandCompanyTitle);
        }
        printString("> ");

        readString(input);
        splitCommand(input, cmd, arg);

        if (mstrcmp(cmd, "help")) {
            printString("Perintah yang tersedia:\r\n");
            printString("help                 : Tampilkan bantuan\r\n");
            printString("user <username>      : Ganti username\r\n");
            printString("user                 : Reset username ke 'user'\r\n");
            printString("grandcompany <name>  : Ganti warna dan company\r\n");
            printString("clear                : Bersihkan layar, reset warna\r\n");
            printString("add <x> <y>          : Hitung x + y\r\n");
            printString("sub <x> <y>          : Hitung x - y\r\n");
            printString("mul <x> <y>          : Hitung x * y\r\n");
            printString("div <x> <y>          : Hitung x / y\r\n");
            printString("yogurt               : Random kata yogurt\r\n");
            printString("exit                 : Keluar shell\r\n");
        } 
        else if (mstrcmp(cmd, "clear")) {
            clearScreen();
            setTextColor(0x07);
            grandCompanyTitle[0] = 0;
        }
        else if (mstrcmp(cmd, "exit")) {
            printString("Keluar shell...\r\n");
            running = 0;
        }
        else if (mstrcmp(cmd, "user")) {
            if (arg[0] == 0) {
                // Reset ke default
                strcpy(userName, "user");
                printString("Username changed to user\r\n");
            } else {
                // Ganti username
                int len = 0;
                while (arg[len] && len < MAX_NAME - 1) {
                    userName[len] = arg[len];
                    len++;
                }
                userName[len] = 0;
                printString("Username changed to ");
                printString(userName);
                printString("\r\n");
            }
        }
        else if (mstrcmp(cmd, "grandcompany")) {
            if (mstrcmp(arg, "maelstrom")) {
                clearScreen();
                setTextColor(0x04); // merah
                strcpy(grandCompanyTitle, "Storm");
            }
            else if (mstrcmp(arg, "twinadder")) {
                clearScreen();
                setTextColor(0x0E); // kuning
                strcpy(grandCompanyTitle, "Serpent");
            }
            else if (mstrcmp(arg, "immortalflames")) {
                clearScreen();
                setTextColor(0x01); // biru
                strcpy(grandCompanyTitle, "Flame");
            }
            else if (arg[0] == 0 || mstrcmp(arg, "clear")) {
                clearScreen();
                setTextColor(0x07); // default putih
                grandCompanyTitle[0] = 0;
            }
            else {
                printString("Error: Unknown grandcompany name.\r\n");
            }
        }
        else if (mstrcmp(cmd, "add") || mstrcmp(cmd, "sub") || mstrcmp(cmd, "mul") || mstrcmp(cmd, "div")) {
            int x, y;
            if (!parseTwoInts(arg, &x, &y)) {
                printString("Error: Invalid arguments\r\n");
                continue;
            }
            int res = 0;
            if (mstrcmp(cmd, "add")) res = x + y;
            else if (mstrcmp(cmd, "sub")) res = x - y;
            else if (mstrcmp(cmd, "mul")) res = x * y;
            else if (mstrcmp(cmd, "div")) {
                if (y == 0) {
                    printString("Error: Division by zero\r\n");
                    continue;
                }
                res = x / y;
            }
            char buf[32];
            itoa(res, buf, 10);
            printString(buf);
            printString("\r\n");
        }
        else if (mstrcmp(cmd, "yogurt")) {
            int r = (my_interrupt(0x2F, 0, 0, 0, 0) % yogurtCount); // random dengan interrupt 0x2F int 0 (bisa diganti sesuai implementasi)
            printString(yogurtReplies[r]);
            printString("\r\n");
        }
        else if (cmd[0] == 0) {
            // kosong, input kosong, skip
        }
        else {
            // The Echo: print apa yang user ketik
            printString(input);
            printString("\r\n");
        }
    }
}
