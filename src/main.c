#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "records.h"
#include "commands.h"

#define ERROR "null"
#define N_COMMANDS (sizeof(cmds) / sizeof(command))

#define BAD_USAGE "Wrong number of arguments\n"

#define error(...) printf("ERROR: " __VA_ARGS__)

void set(int, char **);
void get(int, char **);
void list(int, char **);
void remove_cmd(int, char **);
void rename_record(int, char **);
void help(int, char **);

command cmds[] = {
    {.name = "get",  .func = get  },
    {.name = "set",  .func = set  },
    {.name = "list", .func = list },
    {.name = "rm",   .func = remove_cmd},
    {.name = "mv",   .func = rename_record},
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

    if(func == NULL) {
        printf("Unknown command: %s\n", cmd);
        printf("Please run fs script instead.\n");
    } else if((manager = load_records()) == NULL)
        // TODO: print basefile name
        printf("Unable to upload records. Please check your base file.\n");
    else {
        func(new_argc, new_argv);
        close_storage(manager);
        return 0;
    }
    return 1;
}


void get(int argc, char ** argv){
    if(argc == 0){
        error(BAD_USAGE);
        printf("usage: fs get <record-name> \n");
        exit(1);
    }

    char * rec_name = argv[0];
    record * r = get_record(manager, rec_name);

    if(r != NULL)
        printf("%s", r->path);
    else
        error("No record with name: %s\n", rec_name);
}

char * resolve_path(char * path){
    if( path == NULL ) return getenv("PWD"); // current directory

    struct stat info;
    if ( stat(path, &info) == -1 )
        perror("ERROR");
    else if( !S_ISDIR(info.st_mode) ) 
        error("Invalid path: %s\n", realpath(path, NULL));
    else
        return realpath(path, NULL);;

    return NULL;
}
void set(int argc, char **argv){
    if(argc < 1){
        error(BAD_USAGE);
        printf("usage: fs set <record-name> [ <record-name> ]\n");
        exit(1);
    } 

    char * rec_name  = argv[0]; 
    char * path      = argc < 2 ? NULL : argv[1];
    char * new_path  = resolve_path(path);

    if(new_path == NULL){
        error("Unable to resolve record path : %s\n", path);
        exit(1);
    }

    char * old_path = create_record(manager, rec_name, new_path);

    if ( old_path != NULL && strcmp(old_path, new_path) == 0 ) {
        printf("record        [ '%s' => '%s' ]\n", rec_name, new_path);
        printf("already exits.\n");
    }else {
        if(old_path == NULL)
            // TODO: look at this print :)
            printf("record [ %s => %s ] added.\n", rec_name, new_path); 
        else {
            printf("replaced record  [ %s => %s ]\n", rec_name, old_path);
            printf("to               [ %s => %s ]\n", rec_name, new_path);
            free(old_path);
        }
        save_records(manager);
    }

    // free(new_path); No need since the program will end any way :)
}

void help(int argc, char ** argv){
    printf("fs-commands: \n");
    printf("    * set <name> [ <path> ] - set's <name> path to <path>\n");
    printf("    * get <name>            - moves to the path of <name> if such exists\n");
    printf("    * rm  <name>            - removes record which name is <name> if such exists\n");
    printf("    * mv  <name> <new-name> - changed record name from <name> to <new-name>\n");
    printf("    * <name>                - alias for get <name>\n");
    printf("    * list                  - list all record entries stored\n");
    printf("    * help                  - prints help message\n");
}

void list(int argc, char ** argv){

    // get's the max lenght of the records name
    // pretty prints
    lnode * curr = manager->records.head;
    size_t max = 0;

    while(curr != NULL){
        size_t lenght = strlen(curr->record.name);
        if(lenght > max) max = lenght;
        curr = curr->next;
    }
    
    if(max == 0)
        printf("No records...\n");
    else {
        // TODO: print heading :(
        char * format = NULL;
        asprintf(&format, "%%-%zus => %%s\n", max);
        curr = manager->records.head;
        // printf(format, "NAME", "PATH");
        while(curr != NULL){
            record rec = curr->record;
            printf(format, rec.name, rec.path);
            curr = curr->next;
        }

        free(format);
    }

}


void remove_cmd(int argc, char ** argv){
    if(argc == 0){
        error(BAD_USAGE);
        printf("usage: fs rm <record-name>\n");
        exit(1);
    }

    int i;
    for(i = 0; i < argc; i++){
        char * name = argv[i];
        char * path = remove_record(manager, name);
        if(path == NULL){
            error("no record with name '%s'\n", name);
            if(i > 0) printf("Restoring all removed records...\n");
            break;
        } else {
            printf("record: [ %s => %s ] removed\n", name, path);
            free(path);
        }
    }

    if( i == argc ) save_records(manager);
    
}


void rename_record(int argc, char ** argv){
    if(argc < 2){
        error(BAD_USAGE);
        printf("usage: fs mv <record-name> <new-record-name>\n");
        exit(1);
    }

    char * src_name    = argv[0];
    char * target_name = argv[1];

    printf("src: %s ; target: %s\n", src_name, target_name);
}
