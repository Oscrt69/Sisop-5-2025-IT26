#include "shell.h"
#include "kernel.h"

int main() {
  clearScreen();
  shell();
}

void printString(char *str)
{
  int i = 0;
  while (str[i] != '\0') {
    _interrupt(0x10, 0x0E00 + str[i], 0, 0, 0);
    i++;
  }
}

void readString(char *buf)
{
  int i = 0;
  char ch;

  while (1) {
    ch = _interrupt(0x16, 0x0000, 0, 0, 0) & 0xFF;

    if (ch == '\r') { 
      buf[i] = '\0';
      _interrupt(0x10, 0x0E00 + '\r', 0, 0, 0); 
      _interrupt(0x10, 0x0E00 + '\n', 0, 0, 0);
      break;
    }
    else if (ch == '\b') { 
      if (i > 0) {
        i--;
        _interrupt(0x10, 0x0E00 + '\b', 0, 0, 0);
        _interrupt(0x10, 0x0E00 + ' ', 0, 0, 0);
        _interrupt(0x10, 0x0E00 + '\b', 0, 0, 0);
      }
    }
    else {
      buf[i] = ch;
      _interrupt(0x10, 0x0E00 + ch, 0, 0, 0); 
      i++;
    }
  }
}

void clearScreen()
{
  int i;
  for (i = 0; i < 80 * 25; i++) {
    _putInMemory(0xB000, i * 2, ' ');     
    _putInMemory(0xB000, i * 2 + 1, 0x07); 
  }
}
