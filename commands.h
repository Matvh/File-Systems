#ifndef COMMANDS_H
#define COMMANDS_H

#define INFO 1
#define SEARCH 2
#define SHOW 3
#define R 4
#define W 5
#define H 6
#define S 7
#define D 8

#include <stdio.h>
#include <string.h>

//empty or --help
//Print available commands
void COMMANDS_printAvailableCommands();

//Missing arguments error
void COMMANDS_printMissingArguments();

//Returns 1 if the number of arguments for the command is OK
//Returns 0 if the number of arguments for the command is KO
int COMMANDS_verifyNumberOfArguments(int num_arguments, int command);

#endif
