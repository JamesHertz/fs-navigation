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

static lnode * create_node(char * name, char * path, lnode * next){
    record rec = {
        .name = strdup(name),
        .path = strdup(path)
    };

    lnode * node = malloc(sizeof(lnode));
    node->record = rec;
    return node;
}

RecordsManager * load_records(){
    static RecordsManager * manager = NULL;

    if(manager == NULL){
        FILE * storage = get_config_file(); 

        size_t size = 0;
        lnode dummy = {.next = NULL};
        lnode * curr = &dummy;

        // uplods the records :)
        char * line = NULL;
        size_t line_size = 0;

        while(getline(&line, &line_size, storage) > 0){
            char * rec_name = strtok(line, REC_SEP);
            char * rec_path = strtok(NULL, "\n");
            curr = curr->next = create_node(rec_name, rec_path, NULL);
            ++size;
        }

        if(line != NULL) free(line);

        manager = malloc(sizeof(RecordsManager));
        manager->storage = storage;
        manager->records = (llist) {
            .head = dummy.next,
            .tail = (size == 0) ? NULL : curr,
            .size = size
        };
    }

    return manager;
}

void save_records(const RecordsManager *m){
    FILE * storage = m->storage;

    fseek(storage, 0, SEEK_SET);
    ftruncate(fileno(storage), 0);

    lnode * curr = m->records.head;
    while (curr != NULL){
        record r = curr->record;
        fprintf(storage, REC_LINE_FORMAT, r.name, r.path);
        curr = curr->next;
    }
    fflush(storage);
}

static lnode * get_node(const RecordsManager *m, char * name){
    lnode * curr = m->records.head;

    while(curr != NULL){
        record * rec = &curr->record;
        if(!strcmp(rec->name, name)) 
            return curr;
        curr = curr->next;
    }
    return NULL;
}

record * get_record(const RecordsManager *m, char * name){
    lnode * node = get_node(m, name);
    return node == NULL ? NULL : &node->record;
}

char * create_record(RecordsManager *m, char * name, char * path){
    record * rec = get_record(m, name);
    char * old_rec_path = NULL;
    if(rec == NULL) {
        llist * list = &m->records;
        lnode * new_node = create_node(name, path, NULL);

        if(list->size == 0)
            list->head = new_node;
        else
            list->tail->next = new_node;

        list->tail = new_node;
        list->size += 1;
    }else{
        old_rec_path = rec->path;
        rec->path = strdup(path);
    }

    return old_rec_path;
}

char * remove_record(RecordsManager * m, char * name){
    lnode ** curr = &m->records.head;
    lnode * prev = NULL;

    while(*curr!= NULL){
        if(!strcmp((*curr)->record.name, name)){
            lnode * aux = *curr;

            if(aux == m->records.tail) // change tail :)
                m->records.tail = prev;

            // updates the next of the last node
            *curr = (*curr)->next;

            record rec = aux->record;
            free(rec.name);
            free(aux);
            return rec.path;
        }

        prev = *curr;
        curr = &(*curr)->next;
    }
    return NULL;
}

void close_storage(const RecordsManager * m){
    fclose(m->storage);
}