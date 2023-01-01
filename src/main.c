#include <stdio.h>
#include <string.h>
#include "records.h"

#define GET "get"
#define SET "set"
#define HELP "help"
#define LIST "list"

#define ERROR "null"

typedef void (*cmd_func) (int, char **);

typedef struct {
    char * name;
    cmd_func func;
} command;

void get(int, char **);
void set(int, char **);
void help(int, char **);

command commands[] = {
    {.name = "get", .func = get },
    {.name = "set", .func = set },
    {.name = "help", .func = help}
};


int cmd_length = (sizeof(commands) / sizeof(commands[0]));

cmd_func get_func(char * c_name){

    for(int i = 0; i < cmd_length; i++){
        command * cmd = &commands[i];
        if(!strcmp(cmd->name, c_name)) return cmd->func;
    }

    return NULL;
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

    cmd_func func;

    if(strcmp("", cmd))
        func = help;
    else
        func = get_func(cmd);

    if(func != NULL)
        func(new_argc, new_argv);
    else {
        // TODO some changes...
        printf("Unknown command: %s\n", cmd);
        printf("Run: <program> help - to get the list of all commands\n");
    }
    return 0;
}


void get(int argc, char * argv[]){
    if(argc == 0){
        printf("error running get-command\n");
        printf("should provide: <record-name>\n");
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
        printf("error running set-command\n");
        printf("should provide: <record-name> <record-path>\n");
        return;
    }
    create_record(argv[0], argv[1]);
}

void help(int argc, char * argv[]){
    printf("TO BE DONE!!\n");
}

