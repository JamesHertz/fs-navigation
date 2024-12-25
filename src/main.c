#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "records.h"
#include <assert.h>

// some messages
#define BAD_USAGE "Wrong number of arguments\n"
#define NO_SUCH_RECORD "No record with name: %s\n"
#define RECORD_REMOVED "record: [ %s => %s ] removed\n"
#define RECORD_REPLACED "replaced record  [ %s => %s ]\n" \
                        "to               [ %s => %s ]\n"

#define error(...) printf("[ ERROR ] " __VA_ARGS__)
#define panic(...) do {     \
        error(__VA_ARGS__); \
        exit(EXIT_FAILURE); \
    } while(0)

#define max(x, y) ((x) > (y) ? (x) : (y))

void help(){
    puts("fs-commands: ");
    puts("    * set <name> [ <path> ] - set's <name> path to <path>");
    puts("    * get <name>            - prints the directory associated with <name>");
    puts("    * rm  <name>            - removes record which name is <name> if such exists");
    puts("    * mv  <name> <new-name> - changed record name from <name> to <new-name>");
    puts("    * <name>                - moves to the path of <name> if such exists");
    puts("    * list                  - list all record entries stored");
    puts("    * help                  - prints help message");
}

void list(const RecordsManager * m){
    if(rm_records_number(m) == 0) 
        puts("No records...");
    else {
        size_t max_name_len = 0;
        RM_FOR_EACH_RECORD(m, rec, {
            size_t lenght = strlen(rec.name);
            max_name_len = max(max_name_len, lenght);
        });

        // All of this work to make the output look a bit prettier
#define NAME_TITLE "Record Name"
#define PATH_TITLE "    Record Path"
        max_name_len = max(max_name_len, sizeof(NAME_TITLE) - 1);
        printf("%-*s" PATH_TITLE "\n",  (int) max_name_len, NAME_TITLE);

        for(int i = 0; i < max_name_len + sizeof(PATH_TITLE) -1; i++) { putchar('-'); }
        putchar('\n');
#undef NAME_TITLE
#undef PATH_TITLE

        RM_FOR_EACH_RECORD(m, rec, {
            printf("%-*s => %s\n", (int) max_name_len, rec.name, rec.path);
        });
    }
}

int remove_record(RecordsManager * m, int argc, char ** argv){
    if(argc == 0){
        fputs("usage: fs rm <record-name>...\n", stderr);
        return 1;
    }

    int i;
    for(i = 0; i < argc; i++){
        char * name = argv[i];
        char * old_path = rm_remove_record(m, name);
        if(old_path == NULL){
            error("No record named '%s'\n", name);
            return 1;
        } else {
            printf(RECORD_REMOVED, name, old_path);
            free(old_path);
         }
    }

    return 0;
}


int get_record_path(RecordsManager * m, int argc, char ** argv){
    if(argc == 0){
        fputs("usage: fs get <record-name>\n", stderr);
        return 1;
    }

    char * rec_name = argv[0];
    const char * path = rm_find_path(m, rec_name);

    if(path != NULL)
        fputs(path, stdout);
    else
        error(NO_SUCH_RECORD, rec_name);

    return 0;
}

char * get_full_path(char * path){
    if( path == NULL ) {
        path = getenv("PWD");
        assert(path && "PWD env var not defined");
        return strdup(path);
    }

    errno = 0;
    struct stat info;
    if ( stat(path, &info) == -1 )
        error("Resolving path: %s\n", strerror(errno));
    else if( !S_ISDIR(info.st_mode) ) 
        error("Path should be a directory: %s\n", realpath(path, NULL));
    else
        return realpath(path, NULL);;

    return NULL;
}

int set_new_record(RecordsManager * m,int argc, char **argv){
    if(argc < 1){
        fputs("usage: fs set <record-name> [ <record-name> ]\n", stderr);
        return 1;
    } 

    char * rec_name = argv[0]; 
    char * new_path = get_full_path(argc < 2 ? NULL : argv[1]);

    if(new_path == NULL) return 1;

    const char * old_path = rm_find_path(m, rec_name);
    if ( old_path != NULL && strcmp(old_path, new_path) == 0 ) {
        printf("record        [ '%s' => '%s' ]\n", rec_name, new_path);
        printf("already exits.\n");
    } else {
        old_path = rm_put_record(m, rec_name, new_path);
        if(old_path == NULL)
            printf("record [ %s => %s ] added.\n", rec_name, new_path); 
        else {
            printf(RECORD_REPLACED, rec_name, old_path, rec_name, new_path);
            free((void *) old_path);
        }
    }
    return 0;
}

int rename_record(RecordsManager * m, int argc, char ** argv){
    if(argc < 2){
        fputs("usage: fs mv <record-name> <new-record-name>\n", stderr);
        return 1;
    }

    char * src_name    = argv[0];
    char * target_name = argv[1];

    char * path_name   = rm_remove_record(m, src_name);
    if( path_name == NULL ) {
        error(NO_SUCH_RECORD, src_name);
        return 1;
    } else {
        char * target_path = rm_remove_record(m, target_name);
        if ( target_path != NULL ) {
            printf(RECORD_REMOVED, target_name, target_path);
            free(target_path);
        }

        printf(RECORD_REPLACED, src_name, src_name, target_name, target_name);
        free(path_name);
    }
    return 0;
}

RecordsManager * load_records() {
    errno = 0;

    char * base_file_name = resolve_base_filename();
    if(base_file_name == NULL)
        panic("Resolving base file name: %s", strerror(errno));

    printf("loading files from: %s\n", base_file_name);
    RecordsManager * records = rm_load_records(base_file_name);
    if(records == NULL)
        panic("Loading records from '%s': %s", base_file_name, strerror(errno));

    free(base_file_name);
    return records;
}

int main(int argc, char * argv[]){
    // TODO:
    // - Do the optmizations about mallocs
    // - Improve the install script (the simplest the better)
    // - Make main.sh receive the path on the stdin but send all the errors on stderr
    // - Think about using the executable as a database and the interface be made in main.sh

    char * cmd = NULL;
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

    int status = 0;
    if(cmd == NULL || strcmp(cmd, "help") == 0)
        help();
    else {
        RecordsManager * m = load_records();
        if(strcmp(cmd, "list") == 0)
            list(m);
        else if(strcmp(cmd, "rm") == 0)
            status = remove_record(m, new_argc, new_argv);
        else if(strcmp(cmd, "get") == 0)
            status = get_record_path(m, new_argc, new_argv);
        else if(strcmp(cmd, "set") == 0)
            status = set_new_record(m, new_argc, new_argv);
        else if(strcmp(cmd, "mv") == 0)
            status = rename_record(m, new_argc, new_argv);
        else {
            fprintf(stderr, "Unknown command: '%s'.\nPlease run 'fs help' to list all commands\n", cmd);
            status = 1;
        }

        rm_destroy(m);
    }
    return status;
}
