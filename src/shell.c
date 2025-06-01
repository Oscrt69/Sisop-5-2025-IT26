#include "shell.h"
#include "kernel.h"
#include "std_lib.h"

char current_user[32] = "user";
char grand_company[32] = "";
byte current_color = 0x07;

void shell(){
  char buf[128];
  char cmd[64];
  char args[2][64];
  printString("Welcome to AmbatubOS!\n");
  while (true) {
        _interrupt(0x10, 0x0E00 | current_color, 0, 0, 0);
        printString(current_user);
        printString(grand_company);
        printString("> ");
        _interrupt(0x10, 0x0E00 | 0x07, 0, 0, 0);
        readString(buf);
        parseCommand(buf, cmd, args);
        executeCommand(cmd, args);
  }
}

void parseCommand(char *buf, char *cmd, char arg[2][64]) {
  int i = 0, j = 0, arg_num = 0;
    while (buf[i] == ' ') i++;
    while (buf[i] != ' ' && buf[i] != '\0') {
        cmd[j++] = buf[i++];
    }
    cmd[j] = '\0';
    
    while (buf[i] != '\0' && arg_num < 2) {
        while (buf[i] == ' ') i++;
        if (buf[i] == '\0') break;
        
        j = 0;
        while (buf[i] != ' ' && buf[i] != '\0') {
            arg[arg_num][j++] = buf[i++];
        }
        arg[arg_num][j] = '\0';
        arg_num++;
    }
}

void executeCommand(char *cmd, char arg[2][64]) {
    // Nomer 1
    if (!strcmp(cmd, "echo")) {
        printString(arg[0]);
        printString("\n");
    }
    // Nomer 2
    else if (strcmp(cmd, "yo")) {
        printString("gurt\n");
    } else if (strcmp(cmd, "gurt")) {
        printString("yo\n");
    }
    // Nomer 3
    else if (strcmp(cmd, "user")) {
        if (strcmp(arg[0], "")) {
            strcpy(current_user, arg[0]);
        } else {
            strcpy(current_user, "user");
        }
    }
    // Nomer 4
    else if (strcmp(cmd, "grandcompany")) {
        if (strcmp(arg[0], "maelstrom")) {
            current_color = 0x04; // Merah
            strcpy(grand_company, "@Storm");
        } else if (strcmp(arg[0], "twinadder")) {
            current_color = 0x0E; // Kuning
            strcpy(grand_company, "@Serpent");
        } else if (strcmp(arg[0], "immortalflames")) {
            current_color = 0x01; // Biru
            strcpy(grand_company, "@Flame");
        } else {
            printString("Invalid Grand Company\n");
        }
        clearScreen();
    } else if (strcmp(cmd, "clear")) {
        current_color = 0x07; // Putih
        grand_company[0] = '\0';
        clearScreen();
    }
    // Nomer 5
    else if (strcmp(cmd, "add") || strcmp(cmd, "sub") || 
             strcmp(cmd, "mul") || strcmp(cmd, "div")) {
        int x, y;
        atoi(arg[0], &x);
        atoi(arg[1], &y);
        
        int result;
        if (strcmp(cmd, "add")) result = x + y;
        else if (strcmp(cmd, "sub")) result = x - y;
        else if (strcmp(cmd, "mul")) result = x * y;
        else if (y != 0) result = div(x, y);
        else {
            printString("Error: Division by zero\n");
            return;
        }
        
        char str[16];
        itoa(result, str);
        printString(str);
        printString("\n");
    }
    // Nomer 6
    else if (strcmp(cmd, "yogurt")) {
        const char* responses[] = {"yo\n", "ts unami gng </3\n", "sygau\n"};
        int idx = mod(getBiosTick(), 3);
        printString(responses[idx]);
    }
    //default
    else {
        printString(cmd);
        printString("\n");
    }
}
