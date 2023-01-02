#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "records.h"

#define ERROR "null"

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

void list(int argc, char * argv[]){

    lnode it = get_records_iterator();

    while(it != NULL){
        record rec = it_node_record(it);
        printf("%s - %s\n",rec->name, rec->path);
        it = it_next(it);
    }    

}

#define N_COMMANDS (sizeof(commands) / sizeof(commands[0]))

static command commands[] = {
    {.name = "get",  .func = get  },
    {.name = "set",  .func = set  },
    {.name = "list", .func = list },
    {.name = "help", .func = help }
};

cmd_func get_func(char * c_name){
    if(!strcmp("", c_name)) return help;

    for(int i = 0; i < N_COMMANDS; i++){
        command * cmd = &commands[i];
        if(!strcmp(cmd->name, c_name)) return cmd->func;
    }

    return NULL;
}