#include "shell.h"
#include "kernel.h"

int main() {
  clearScreen(0x07);  
  shell();
}

void printString(char *str) {
    while (*str != '\0') {
        interrupt(0x10, 0x0E00 + *str, 0, 0, 0);        
        str++;
    }
}


void readString(char *buf) {
    char ch;
    int i = 0;
    while (1) {
        ch = interrupt(0x16, 0x0000, 0, 0, 0) & 0xFF;
        if (ch == 0x0D) break;
        if (ch == 0x08) {
            if (i > 0) {
                i--;
                printString("\b \b");
            }
        } else {
            buf[i++] = ch;
            interrupt(0x10, 0x0E00 + ch, 0, 0, 0);
        }
    }
    buf[i++] = 0x0D;
    buf[i++] = 0x0A;
    buf[i] = '\0';  
    printString("\r\n");
}

void clearScreen(int color)
{
  int i;
  for (i = 0; i < 80 * 25 * 2; i += 2) {
    putInMemory(0xB800, i, ' ');
    putInMemory(0xB800, i + 1, color); 
  }
  interrupt(0x10, 0x0200, 0, 0, 0);
}
