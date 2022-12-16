#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "records.h"

#define GET "get"
#define SET "set"
#define HELP "help"

void get(int argc, char * argv[]){
    printf("running get\n");
}

void set(int argc, char * argv[]){
    printf("running set\n");
}

void help(){
    printf("TO BE DONE!!\n");
}

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

    if(!strcmp("", cmd) || !strcmp(HELP, cmd)) help();
    else if(!strcmp(cmd, GET))
        get(new_argc, new_argv);
    else if(!strcmp(cmd, SET))
        set(new_argc, new_argv);
    else {
        printf("Unknown command: %s\n", cmd);
        printf("Run: <program> help - to get the list of all commands\n");
    }
    return 0;
}