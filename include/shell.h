#ifndef __SHELL_H__
#define __SHELL_H__

#include "std_type.h"

void shell();
void parseCommand(char *buf, char *cmd, char arg[2][64]);
void runCommand(char *cmd, char *buf, char arg[2][64]);
void prompt(char *word);
void changeColor(int color);
void randomAnswer();

#endif // __SHELL_H__
