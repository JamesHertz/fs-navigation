#include <stdio.h>
#include <string.h>
#include "records.h"
#include "commands.h"

#define ERROR "null"
#define N_COMMANDS (sizeof(cmds) / sizeof(command))

void set(int, char **);
void get(int, char **);
void list(int, char **);
void help(int, char **);

command cmds[] = {
    {.name = "get",  .func = get  },
    {.name = "set",  .func = set  },
    {.name = "list", .func = list },
    {.name = "help", .func = help }
};

RecordsManager * manager;

int main(int argc, char * argv[]){

    commands cmd_store = {
        .count = N_COMMANDS,
        .commands = cmds,
    };

    manager = load_records();

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

    cmd_func func = (*cmd == '\0') ? help : get_func(&cmd_store, cmd);

    if(func != NULL)
        func(new_argc, new_argv);
    else {
        // TODO some changes...
        printf("Unknown command: %s\n", cmd);
        printf("Run: <program> help - to get the list of all commands\n");
    }
    return 0;
}

void get(int argc, char ** argv){
    if(argc == 0){
        printf("error running get-command\n");
        printf("should provide: <record-name>\n");
        return;
    }

    record * r = get_record(manager, argv[0]);
    if(r != NULL)
        printf("%s", r->path);
    else
        printf(ERROR);

}

void set(int argc, char **argv){
    if(argc < 2){
        printf("error running set-command\n");
        printf("should provide: <record-name> <record-path>\n");
        return;
    }
    create_record(manager, argv[0], argv[1]);
}

void help(int argc, char ** argv){
    printf("TO BE DONE!!\n");
}

void list(int argc, char ** argv){

    //printf("%s - %s\n",rec->name, rec->path);

}