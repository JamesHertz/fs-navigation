#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "records.h"

#define REC_SEP  "\t"
#define REC_LINE_FORMAT "%s" REC_SEP "%s\n"

#define DEFAULT_BASE_FILE ".fs-nav"
#define HOME_DIR "HOME"
#define BASE_FILE "FS_BASE_FILE"

static FILE * get_base_file(){
    char * base_file_name = getenv(BASE_FILE);
    char * aux = NULL;

    if(base_file_name == NULL){
        char * home_dir = getenv(HOME_DIR);
        if(home_dir == NULL) return NULL;
        asprintf(&aux, "%s/%s", home_dir, DEFAULT_BASE_FILE);
        base_file_name = aux;
    }

    FILE * file = fopen(base_file_name, "r+");
    if(file == NULL && errno == ENOENT)
        file = fopen(base_file_name, "w+"); // create's the file
                                            //
    if(aux != NULL) free(aux);

    return file;
}

static lnode * create_node(char * name, char * path, lnode * next){
    // little optimization :)
    record rec = {
        .name = strdup(name),
        .path = strdup(path)
    };

    lnode * node = malloc(sizeof(lnode));
    node->record = rec;
    node->next = next;
    return node;
}

RecordsManager * load_records(){
    static RecordsManager * manager = NULL; // Am I not over optimizing?

    if(manager == NULL){
        FILE * storage = get_base_file(); 

        if(storage == NULL) return NULL;

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
    // fileno(FILE * f) returns the file descriptor of a FILE
    ftruncate(fileno(storage), 0); // truncates the file

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
