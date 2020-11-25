#include "commands.h"

void COMMANDS_printInfo(){
    printf("--info: \n");
    printf("  --info <volume_name>\n");
}

void COMMANDS_printSearch(){
    printf("--search: \n");
    printf("  --search <volume_name> <file_name>\n");
}

void COMMANDS_printShow(){
    printf("--show: \n");
    printf("  --show <volume_name> <file_name>\n");
}

void COMMANDS_printFileProperties(){
    printf("-r: read only\n");
    printf("  -r <volume_name> <file_name>\n");
    printf("\n");
    printf("-w: write only\n");
    printf("  -w <volume_name> <file_name>\n");
    printf("\n");
    printf("-h: activate hidden file mode\n");
    printf("  -h <volume_name> <file_name>\n");
    printf("\n");
    printf("-s: deactivate hidden file mode\n");
    printf("  -s <volume_name> <file_name>\n");
    printf("\n");
    printf("-d: modify creation date\n");
    printf("  -d <new_date> <volume_name> <file_name>\n");
    printf("\n\n");
}

void COMMANDS_printAvailableCommands(){
    printf("\n");
    printf("Usage: ./infinityWar <command> <volume_name> [<file_name>]\n");
    printf("\n");
    COMMANDS_printInfo();
    printf("\n");
    COMMANDS_printSearch();
    printf("\n");
    COMMANDS_printShow();
    printf("\n");
    COMMANDS_printFileProperties();
}

int COMMANDS_verifyNumberOfArguments(int num_arguments, int command){

    switch (command) {
        case INFO:
            if (num_arguments < 3) return 0;
            else return 1;
        case SEARCH:
        case SHOW:
        case R:
        case 5:
        case H:
        case S:
            if (num_arguments < 4) return 0;
            else return 1;
        case D:
            if (num_arguments < 5) return 0;
            else return 1;
        default:
         return 0;
     }
}

void COMMANDS_printMissingArguments(){
    printf("\nError: Missing arguments for this command\n");
    printf("Use --help to check how to use the command\n\n");
}
