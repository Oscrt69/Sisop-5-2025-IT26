#include "shell.h"
#include "kernel.h"
#include "std_lib.h"

char user[64] = "user"; 
char host[64] = ""; 
int textColor = 0x07;

void shell(){
  char buf[128];
  char cmd[64];
  char arg[2][64];
  printString("This is IT-26 OS!\r\n");
  while (true) {
    char prompt[128];
    strcpy(prompt, user);
    strcat(prompt, host);
    strcat(prompt, "> ");
    printString(prompt);
    readString(buf);
    parseCommand(buf, cmd, arg);
    runCommand(cmd,buf,arg);    
  }
}

void parseCommand(char *buf, char *cmd, char arg[2][64]) {
    int i = 0, j = 0, k = 0;
    int argIndex = 0;
    while (buf[i] != '\0' && (buf[i] == '\r' || buf[i] == '\n')) i++;
    while (buf[i] != '\0' && buf[i] != ' ' && buf[i] != '\r' && buf[i] != '\n') {
        cmd[j++] = buf[i++];
    }
    cmd[j] = '\0';   
    while (buf[i] == ' ') i++;   
    for (argIndex = 0; argIndex < 2; argIndex++) {
        k = 0;
        while (buf[i] != '\0' && buf[i] != ' ' && buf[i] != '\r' && buf[i] != '\n') {
            arg[argIndex][k++] = buf[i++];
        }
        arg[argIndex][k] = '\0';
       
        while (buf[i] == ' ') i++;
    }
}

void changeColor(int color) {
    textColor = color;
}

void prompt(char *word) {
  printString(word);
  if (host[0] != '\0') {
    printString(host);  
  }
  printString("> ");
}

void randomAnswer() {    
    int r = mod(getBiosTick(),3);
    if (r == 0) {
        printString("ts unami gng </3\r\n");
    } else if (r == 1) {
        printString("yo\r\n");
    } else {
        printString("sygau\r\n");
    }    
}


void runCommand(char *cmd, char *buf, char arg[2][64]) {
    int a, b, result;
    char result_str[32];
    if (strcmp(cmd, "yo")) {
        printString("gurt\r\n");

    } else if (strcmp(cmd, "gurt")) {
        printString("yo\r\n");

    } else if (strcmp(cmd, "user")) {
        if (arg[0][0] == '\0') {
            strcpy(user, "user");
        } else {
            strcpy(user, arg[0]);
        }
        printString("Username changed to ");
        printString(user);
        printString("\r\n");

    } else if (strcmp(cmd, "grandcompany")) {        
        if (strcmp(arg[0], "maelstrom")) {  
          changeColor(0x0c);   
          clearScreen(textColor);                       
          strcpy(host, "@Storm");            
        } else if (strcmp(arg[0], "twinadder")) {   
          changeColor(0x0e);   
          clearScreen(textColor);             
          strcpy(host, "@Serpent");            
        } else if (strcmp(arg[0], "immortalflames")) {       
          changeColor(0x09);   
          clearScreen(textColor);             
          strcpy(host, "@Flame");                   
        } else {
            printString("Unknown\r\n");
        }

    } else if (strcmp(cmd, "clear")) {
      strcpy(host, "");
      changeColor(0x07);
      clearScreen(textColor);      

    } else if (strcmp(cmd, "add")){
        if (arg[0][0] == '\0' || arg[1][0] == '\0') {
            printString("Usage: add <num1> <num2>\r\n");
            return;
        }
        atoi(arg[0], &a);
        atoi(arg[1], &b);
        result = a + b;
        itoa(result, result_str);        
        printString(result_str);
        printString("\r\n");

    } else if (strcmp(cmd, "sub")) {
        if (arg[0][0] == '\0' || arg[1][0] == '\0') {
            printString("Usage: sub <num1> <num2>\r\n");
            return;
        }
        atoi(arg[0], &a);
        atoi(arg[1], &b);
        result = a - b;
        itoa(result, result_str);        
        printString(result_str);
        printString("\r\n");

    } else if (strcmp(cmd, "mul")){  
      if (arg[0][0] == '\0' || arg[1][0] == '\0') {
          printString("Usage: mul <num1> <num2>\r\n");
          return;
      }
      atoi(arg[0], &a);
      atoi(arg[1], &b);
      result = a * b;
      itoa(result, result_str);      
      printString(result_str);
      printString("\r\n");

    } else if (strcmp(cmd, "div")){    
      if (arg[0][0] == '\0' || arg[1][0] == '\0') {
          printString("Usage: div <num1> <num2>\r\n");
          return;
      }
      atoi(arg[0], &a);
      atoi(arg[1], &b);
      if (b == 0) {
          printString("Undefined\r\n");
          return;
      }
      result = div(a, b);
      itoa(result, result_str);      
      printString(result_str);
      printString("\r\n");
      atoi(arg[0], &a);
      atoi(arg[1], &b);
      result = a + b;
   
    } else if (strcmp(cmd, "yogurt")){
      prompt("gurt");
      randomAnswer();    
    } else {        
        printString(buf);
    }
}
