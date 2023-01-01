#include <stdio.h>
#include "commands.h"

int main(int argc, char * argv[]){

    char * cmd = "";
    char ** new_argv = NULL;
    int new_argc = 0;

    for(int i = 1; i < argc; i++){
        if(i == 1) cmd = argv[i];
        else if (i == 2){
            new_argv = &argv[i];
            new_argc = argc - i; // new argc
            break;
        } 
    }

    cmd_func func = get_func(cmd);

    if(func != NULL)
        func(new_argc, new_argv);
    else {
        // TODO some changes...
        printf("Unknown command: %s\n", cmd);
        printf("Run: <program> help - to get the list of all commands\n");
    }
    return 0;
}