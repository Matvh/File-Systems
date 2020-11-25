#include <stdio.h>
#include "commands.h"
#include "FileSystem/filesystem.h"

#include <string.h>

//TODO usar debugfs

int main(int argv, char * argc[]){

    //verify number of arguments
    if (argv < 2) {
        COMMANDS_printAvailableCommands();
        return 0;
    }

    //Verify each command + number of arguments for each command
    if (strcmp(argc[1], "--info") == 0) {
        if(COMMANDS_verifyNumberOfArguments(argv, INFO))FILESYSTEM_showInfo(argc[2]);
        else COMMANDS_printMissingArguments();

    }else if(strcmp(argc[1], "--search") == 0){
        if(COMMANDS_verifyNumberOfArguments(argv, SEARCH)){
            FILESYSTEM_searchFile(argc[2], argc[3]);
        }
        else COMMANDS_printMissingArguments();

    }else if(strcmp(argc[1], "--show") == 0){
        if(COMMANDS_verifyNumberOfArguments(argv, SHOW)){
            FILESYSTEM_showFileContent(argc[2], argc[3]);
        }else COMMANDS_printMissingArguments();

    }else if(strcmp(argc[1], "-r") == 0){
        if(COMMANDS_verifyNumberOfArguments(argv, R)){
            //TODO
        }else COMMANDS_printMissingArguments();

    }else if (strcmp(argc[1], "-w") == 0) {
        if(COMMANDS_verifyNumberOfArguments(argv, W)){
            //TODO
        }else COMMANDS_printMissingArguments();

    }else if (strcmp(argc[1], "-h") == 0){
        if(COMMANDS_verifyNumberOfArguments(argv, INFO)){
            //TODO
        }else COMMANDS_printMissingArguments();

    }else if(strcmp(argc[1], "-s") == 0){
        if(COMMANDS_verifyNumberOfArguments(argv, INFO)){
            //TODO
        }else COMMANDS_printMissingArguments();

    }else if(strcmp(argc[1], "-d") == 0){
        if(COMMANDS_verifyNumberOfArguments(argv, D)){
            //TODO
        }else COMMANDS_printMissingArguments();

    }
    else if(strcmp(argc[1], "--help") == 0) COMMANDS_printAvailableCommands();
    else printf("\nCommand %s not recognized. Use --help to see the available commands\n\n", argc[1]);

    return 0;
}
