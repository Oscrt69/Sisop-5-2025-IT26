#include "std_lib.h"
#include "kernel.h"

extern unsigned short my_interrupt(unsigned char int_num, unsigned short ax, unsigned short bx, unsigned short cx, unsigned short dx);

#define INPUT_SIZE 128
#define NAME_SIZE 32

char currentUser[NAME_SIZE] = "user";
char companyTag[16] = "";
byte currentColor = 0x07;

char* yogurtPhrases[] = {
    "yo", "ts unami gng </3", "sygau", "gurt", "yosh", "hakuna matata"
};
int yogurtTotal = sizeof(yogurtPhrases) / sizeof(char*);

void putChar(char c) {
    my_interrupt(0x10, 0x0E00 | c, currentColor, 0, 0);
}

void putString(char* str) {
    while (*str) putChar(*str++);
}

void clear() {
    my_interrupt(0x10, 0x0600, 0x0700, 0, 0x184F);
}

void setColor(byte color) {
    currentColor = color;
    my_interrupt(0x10, 0x0B00 | color, 0, 0, 0);
}

void inputLine(char* buffer) {
    int i = 0;
    char c;
    while (i < INPUT_SIZE - 1) {
        unsigned short key = my_interrupt(0x16, 0, 0, 0, 0);
        c = key & 0xFF;

        if (c == '\r') {
            buffer[i] = 0;
            putString("\r\n");
            break;
        } else if (c == 0x08 && i > 0) {
            i--;
            putString("\b \b");
        } else if (c >= 32 && c <= 126) {
            buffer[i++] = c;
            putChar(c);
        }
    }
    buffer[i] = 0;
}

int stringsEqual(char* a, char* b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++; b++;
    }
    return *a == 0 && *b == 0;
}

void parseCommand(char* input, char* cmd, char* args) {
    int i = 0, j = 0;
    while (input[i] && input[i] != ' ' && i < INPUT_SIZE - 1) {
        cmd[i] = input[i];
        i++;
    }
    cmd[i] = 0;
    while (input[i] == ' ') i++;
    while (input[i] && j < INPUT_SIZE - 1) {
        args[j++] = input[i++];
    }
    args[j] = 0;
}

int getTwoNumbers(char* args, int* a, int* b) {
    int i = 0, k = 0;
    char buf1[32] = {0}, buf2[32] = {0};

    while (args[i] && args[i] != ' ') {
        if (k >= 31) return 0;
        buf1[k++] = args[i++];
    }
    buf1[k] = 0;
    while (args[i] == ' ') i++;
    k = 0;
    while (args[i] && k < 31) {
        buf2[k++] = args[i++];
    }
    buf2[k] = 0;

    if (!buf1[0] || !buf2[0]) return 0;
    *a = atoi(buf1);
    *b = atoi(buf2);
    return 1;
}

void showHelp() {
    putString("Daftar perintah:\r\n");
    putString("help                  : Bantuan\r\n");
    putString("user <nama>           : Ganti username\r\n");
    putString("user                  : Reset username\r\n");
    putString("grandcompany <nama>   : Ganti company\r\n");
    putString("clear                 : Hapus layar\r\n");
    putString("add/sub/mul/div x y   : Kalkulator\r\n");
    putString("yogurt                : Respon acak\r\n");
    putString("exit                  : Keluar shell\r\n");
}

void shell() {
    char input[INPUT_SIZE], command[32], args[INPUT_SIZE];
    int aktif = 1;

    putString("Selamat datang di EorzeOS\r\n");
    putString("Ketik 'help' untuk bantuan\r\n");

    while (aktif) {
        putString(currentUser);
        if (companyTag[0]) {
            putChar('@');
            putString(companyTag);
        }
        putString("> ");

        inputLine(input);
        parseCommand(input, command, args);

        if (stringsEqual(command, "help")) {
            showHelp();
        }
        else if (stringsEqual(command, "clear")) {
            clear();
            setColor(0x07);
            companyTag[0] = 0;
        }
        else if (stringsEqual(command, "exit")) {
            putString("Keluar dari shell...\r\n");
            aktif = 0;
        }
        else if (stringsEqual(command, "user")) {
            if (args[0] == 0) {
                strcpy(currentUser, "user");
                putString("Username direset ke 'user'\r\n");
            } else {
                int i = 0;
                while (args[i] && i < NAME_SIZE - 1) {
                    currentUser[i] = args[i];
                    i++;
                }
                currentUser[i] = 0;
                putString("Username diganti ke ");
                putString(currentUser);
                putString("\r\n");
            }
        }
        else if (stringsEqual(command, "grandcompany")) {
            if (stringsEqual(args, "maelstrom")) {
                clear();
                setColor(0x04);
                strcpy(companyTag, "Storm");
            } else if (stringsEqual(args, "twinadder")) {
                clear();
                setColor(0x0E);
                strcpy(companyTag, "Serpent");
            } else if (stringsEqual(args, "immortalflames")) {
                clear();
                setColor(0x01);
                strcpy(companyTag, "Flame");
            } else if (args[0] == 0 || stringsEqual(args, "clear")) {
                clear();
                setColor(0x07);
                companyTag[0] = 0;
            } else {
                putString("Grand Company tidak dikenali\r\n");
            }
        }
        else if (stringsEqual(command, "add") || stringsEqual(command, "sub") ||
                 stringsEqual(command, "mul") || stringsEqual(command, "div")) {
            int x, y;
            if (!getTwoNumbers(args, &x, &y)) {
                putString("Argumen salah\r\n");
                continue;
            }
            int result = 0;
            if (stringsEqual(command, "add")) result = x + y;
            else if (stringsEqual(command, "sub")) result = x - y;
            else if (stringsEqual(command, "mul")) result = x * y;
            else if (stringsEqual(command, "div")) {
                if (y == 0) {
                    putString("Pembagian dengan nol\r\n");
                    continue;
                }
                result = x / y;
            }
            char buf[32];
            itoa(result, buf, 10);
            putString(buf);
            putString("\r\n");
        }
        else if (stringsEqual(command, "yogurt")) {
            int idx = my_interrupt(0x2F, 0, 0, 0, 0) % yogurtTotal;
            putString(yogurtPhrases[idx]);
            putString("\r\n");
        }
        else if (command[0] == 0) {
            // input kosong
        }
        else {
            // The Echo
            putString(input);
            putString("\r\n");
        }
    }
}
