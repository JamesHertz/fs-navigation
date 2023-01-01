#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "records.h"

#define HOME "HOME"
#define CONFIG_FILE_NAME ".fs-nav"


static FILE * cfg_file = NULL;

static FILE * get_config_file(){

    if(cfg_file == NULL){
        //char * home_dir = getenv(HOME);
        char * home_dir = getenv("TEST_DIR");
        char * file_full_name = NULL; // config file full name

        asprintf(&file_full_name, "%s/%s", home_dir, CONFIG_FILE_NAME);

        FILE * file = fopen(file_full_name, "r+");
        if(file == NULL && errno == ENOENT)
            file = fopen(file_full_name, "w+"); // create's the file

        free(file_full_name);
        cfg_file = file;
    }
   
    return cfg_file;
}

typedef struct __lnode {
    record rec;
    struct __lnode * next;     
} * lnode; 

static lnode new_lnode(record rec, lnode next){
    lnode node = malloc(sizeof(struct __lnode));
    node->rec = rec;
    node->next = next;
    return node;
}

static record new_record(char * name, char * path){
    record rec = malloc(sizeof(struct __record__)); 
    rec->name = name;
    rec->path = path;
    return rec;
}

static lnode upload_records(){
    FILE * file = get_config_file();

    struct __lnode dummy = {.next = NULL};
    lnode curr = &dummy;

    char * line = NULL;
    size_t size = 0;
    while(getline(&line, &size, file) > 0){
        char * rec_name = strtok(line, " \t");
        char * rec_path = strtok(NULL, "\n");
        record rec = new_record(
            strdup(rec_name),
            strdup(rec_path)
        );
        curr = curr->next = new_lnode(rec, NULL);
    }

    if(line != NULL) free(line);

    return dummy.next;
}

void store_records(lnode records){
    // TODO: how to trunk the file content??
}

void close_cfg_file(){
    if(cfg_file != NULL) {
        fclose(cfg_file);
        cfg_file = NULL;
    }
}

record get_record(char * name){
    lnode node = upload_records();

    while(node != NULL){
        record rec = node->rec;
        if(!strcmp(rec->name, name))
            return rec;
        node = node->next;
    }


    close_cfg_file();
    return NULL;
}

void create_record(char * name, char * path){
    lnode records = upload_records();  
    record rec = get_record(name);
    if(rec == NULL) {
        FILE * file = get_config_file();
        fprintf(file, "%s %s\n", name, path);
    }else{
        free(rec->path);
        rec->path = strdup(path);
        store_records(records);
    }
    close_cfg_file();

}