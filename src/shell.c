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
  printString("Welcome to EorzeOS!\n");
  while (true) {
    printString("> ");
    readString(buf);
    printString(buf);
    printString("\n");
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
