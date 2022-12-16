#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "records.h"

#define GET "get"
#define SET "set"
#define HELP "help"

#define ERROR "null"

void get(int argc, char * argv[]){
    if(argc == 0){
        //printf("error running get-command\n");
        //printf("should provide: <record-name>\n");
        printf(ERROR);
        return;
    }

    record r = get_record(argv[0]);
    if(r != NULL)
        printf("%s", r->path);
    else
        printf(ERROR);

}

void set(int argc, char * argv[]){
    if(argc < 2){
        //printf("error running set-command\n");
        //printf("should provide: <record-name> <record-path>\n");
        printf(ERROR);
        return;
    }
    // TODO: check if the path exists
    create_record(argv[0], argv[1]);
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