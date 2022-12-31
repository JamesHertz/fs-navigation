#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "records.h"

#define HOME "HOME"
// where our little database will be stored
#define CONFIG_FILE_NAME ".fs-nav"

static FILE * cfg_file = NULL;
static char * file_name = NULL;

static FILE * open_config_file(){

    if(cfg_file != NULL){
      //  char * home_dir = getenv(HOME);
        char * file_full_name = "file.txt"; // config file full name

        //asprintf(&file_full_name, "%s/%s", home_dir, CONFIG_FILE_NAME);

        printf("file-name: %s\n", file_full_name);

        FILE * file = fopen(file_full_name, "r+");
        if(file == NULL && errno == ENOENT)
            file = fopen(file_full_name, "w+"); // create's the file
    }
   
    // TODO: think if it's really worthed worring about the weird error :(    
    // free(file_full_name);
    return cfg_file;
}

static void close_cfg_file(){
    fclose(cfg_file);
}

typedef struct __lnode {
    record * rec;
    struct __lnode * next;     
} * lnode; 

static lnode new_lnode(record * rec, lnode next){
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
    FILE * file = open_config_file();

    struct __lnode dummy = {.next = NULL};
    lnode curr = &dummy;

    char * line = NULL;
    size_t size = 0, len;
    while((len = getline(&line, &size, file)) > 0){
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


record get_record(char * name){
    return NULL;
}

void create_record(char * name, char * path){

}

void print_records(){
    lnode node = upload_records();

    while(node != NULL){
        record rec = node->rec;
        printf("name: %s; path: %s\n", rec->name, rec->path);
    }
}
