#include <stdio.h>
#include <stdlib.h>
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

    if(func != NULL){
        manager = load_records();
        func(new_argc, new_argv);
        close_storage(manager);
    }else {
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
    char * rec_name = argv[0]; 
    char * new_path = argv[1]; 
    char * old_path = create_record(manager, rec_name, new_path);
    if(old_path != NULL){

        if(strcmp(old_path, new_path) != 0){ // if it's different
            printf("record '%s' path was replaced:\n", rec_name);
            printf("%s to %s\n", old_path, new_path);
            save_records(manager);
        }

        free(old_path);
    }

}

void help(int argc, char ** argv){
    printf("TO BE DONE!!\n");
}

void list(int argc, char ** argv){

    // get's the max lenght of the records name
    // pretty prints
    lnode * curr = manager->records.head;
    int max = -1;

    while(curr != NULL){
        int lenght = strlen(curr->record.name);
        if(lenght > max) max = lenght;
        curr = curr->next;
    }
    

    if(max != -1){
        char * format = NULL;
        asprintf(&format, "%%-%ds => %%s\n", max);
        curr = manager->records.head;
        while(curr != NULL){
            record rec = curr->record;
            printf(format, rec.name, rec.path);
            curr = curr->next;
        }

        free(format);
    }

}