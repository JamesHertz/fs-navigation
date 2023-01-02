#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "records.h"


#define REC_SEP  "\t"
#define REC_LINE_FORMAT "%s" REC_SEP "%s\n"

#define CONFIG_FILE_NAME ".fs-nav"

static FILE * get_config_file(){

    char * home_dir = getenv("HOME");
    char * file_full_name = NULL; // config file full name

    asprintf(&file_full_name, "%s/%s", home_dir, CONFIG_FILE_NAME);

    FILE * file = fopen(file_full_name, "r+");
    if(file == NULL && errno == ENOENT)
        file = fopen(file_full_name, "w+"); // create's the file

    free(file_full_name);
    return file;
}

RecordsManager * load_records(){
    static RecordsManager * manager = NULL;

    if(manager == NULL){
        FILE * storage = get_config_file(); 

        llist dummy = {.next = NULL};
        llist * curr = &dummy;

        // uplods the records :)
        char * line = NULL;
        size_t size = 0;
        while(getline(&line, &size, storage) > 0){
            char * rec_name = strtok(line, REC_SEP);
            char * rec_path = strtok(NULL, "\n");
            record rec = {
                .name = strdup(rec_name),
                .path = strdup(rec_path)
            };

            llist * next = malloc(sizeof(llist));
            next->record = rec;
            next->next = NULL;

            curr = curr->next = next;
        }

        if(line != NULL) free(line);

        manager = malloc(sizeof(RecordsManager));
        manager->records = dummy.next;
        manager->storage = storage;
    }

    return manager;
}

//void save_records(RecordsManager * m){
//    FILE * st = m->storage;
//    fseek(st, 0, SEEK_SET);
//    ftruncate(fileno(st) , 0);
//
//    llist *  curr = m->records;
//
//    while(curr != NULL){
//        record rec = curr->rec;
//        fprintf(f, REC_LINE_FORMAT, rec->name, rec->path);
//        curr = curr->next;
//    }
//}

record * get_record(RecordsManager *m, char * name){
    llist * curr = m->records;

    while(curr != NULL){
        record rec = curr->record;
        if(!strcmp(rec.name, name))
            return &rec;
        curr = curr->next;
    }

    return NULL;
}

void create_record(RecordManager *m, char * name, char * path){
    record * rec = get_record(m, name);
    if(rec == NULL) {
//        FILE * file = get_config_file();
//        fseek(file, 0, SEEK_END);
//        fprintf(file, REC_LINE_FORMAT, name, path);
    }else{
//        free(rec->path);
//        rec->path = strdup(path);
//        store_records(records);
    }

    return NULL;
}