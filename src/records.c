#include <stdlib.h>
// #include <errno.h>
#include <string.h>
#include <unistd.h>
#include "records.h"
#include <assert.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define REC_SEP  "\t"
#define REC_LINE_FORMAT "%s" REC_SEP "%s\n"

#define DEFAULT_BASE_FILE ".fs-nav"
#define HOME_DIR "HOME"
#define BASE_FILE "FS_BASE_FILE"

#define INITIAL_RECORDS_SIZE 64
#define TODO()  assert(0 && "TODO")

static void * realloc_memory(void * old, size_t size) {
    void * buffer = realloc(old, size);
    if(buffer == NULL) {
        fprintf(stderr, "[ ERROR ] Failed to (re)allocate %zu bytes\n", size);
        exit(1);
    }
    return buffer;
}

static inline void * alloc_memory(size_t size) {
    return realloc_memory(NULL, size);
}

static void rm_append_record(RecordsManager * m, const char * name, const char * path){
    assert(path && name && "[ ERROR ] Found invalid records");

    if(m->records.length == m->records.capacity) {
        size_t new_capacity = 2 * m->records.capacity;
        // TODO: try to understand the misterios error related with `free(buffer)`
        m->records.items    = realloc_memory(m->records.items, new_capacity * sizeof(RecordsManager));
        m->records.capacity = new_capacity;
    }

    const char * rec_name = strdup(name);
    const char * rec_path = strdup(path);

    assert(rec_path && rec_name);

    Record rec = {
        .name = rec_name,
        .path = rec_path
    };

    int idx = m->records.length++;
    m->records.items[idx] = rec;
}

#define NOT_FOUND -1

static ssize_t rm_find_record_index(const RecordsManager * m, const char * name){
    for(size_t i = 0; i < m->records.capacity; i++){
        Record * rec = &m->records.items[i];
        if(strcmp(rec->name, name))
            return i;
    }
    return NOT_FOUND;
}

static Record * rm_find_record_entry(const RecordsManager * m, const char * name){
    ssize_t idx = rm_find_record_index(m, name);
    return idx == NOT_FOUND ? NULL : &m->records.items[idx];
}

RecordsManager * rm_load_records(const char * base_filename) {
    struct stat st;
    FILE * storage = fopen(base_filename, "r+");

    if(storage == NULL || fstat(fileno(storage), &st) != 0) 
        return NULL;

    // create manager
    RecordsManager * manager = alloc_memory(sizeof(RecordsManager));
    manager->storage = storage;
    manager->dirty   = false;
    manager->records.items    = alloc_memory(sizeof(Record) * INITIAL_RECORDS_SIZE);
    manager->records.capacity = INITIAL_RECORDS_SIZE;
    manager->records.length   = 0;

    if(st.st_size > 0){
        char * buffer = alloc_memory(st.st_size + 1);

        if( fread(buffer, sizeof(char), st.st_size, storage) <= 0 ){
            free(buffer);
            rm_destroy(manager);
            return NULL;
        }

        buffer[st.st_size] = 0;

        char * rec_name = strtok(buffer, REC_SEP);
        char * rec_path = strtok(NULL, "\n");

        // TODO: Think about removing duplicates in case some appear c:
        rm_append_record(manager, rec_name, rec_path);
        while((rec_name = strtok(NULL, REC_SEP)) != NULL){
            rec_path = strtok(NULL, "\n");
            rm_append_record(manager, rec_name, rec_path);
        }

        // TODO: think about optimizations of alloc and frees c:
        free(buffer);
    }

    return manager;
}

const char * rm_find_path(const RecordsManager * m, const char * name)  {
    Record * entry = rm_find_record_entry(m, name);
    return entry == NULL ? NULL : entry->path;
}

const char * rm_put_record(RecordsManager * m, const char * name, const char * path) {
    m->dirty = true;

    Record * entry = rm_find_record_entry(m, name);
    if(entry == NULL) {
        rm_append_record(m, name, path);
        return NULL;
    }

    const char * old_path = entry->path;
    const char * new_path = strdup(path);
    assert(new_path != NULL);
    entry->path = new_path;
    return old_path;
}

const char * rm_remove_record(RecordsManager * m, const char * name) {
    ssize_t idx = rm_find_record_index(m, name);
    if(idx == NOT_FOUND) return NULL;

    m->dirty = true;
    Record rec = m->records.items[idx];
    size_t last_idx = m->records.length - 1;
    if(last_idx >= 0)
        m->records.items[idx] = m->records.items[last_idx];

    m->records.length--;
    free((void *) rec.name);
    return rec.path;
}

int rm_destroy(RecordsManager * m) {
    /*
        if(m->dirty) {
            // TODO: write to file
        }
    */
    fclose(m->storage);
    m->storage = NULL;

    // TODO: write to file
    RM_FOR_EACH_RECORD(m, rec, {
        free((void *) rec.name);
        free((void *) rec.path);
    });

    free(m->records.items);
    m->records.items = NULL;

    free(m);
    return 0;
}

// void save_records(const RecordsManager *m){
//     FILE * storage = m->storage;
//
//     fseek(storage, 0, SEEK_SET);
//     // fileno(FILE * f) returns the file descriptor of a FILE
//     ftruncate(fileno(storage), 0); // truncates the file
//
//     lnode * curr = m->records.head;
//     while (curr != NULL){
//         record r = curr->record;
//         fprintf(storage, REC_LINE_FORMAT, r.name, r.path);
//         curr = curr->next;
//     }
//     fflush(storage);
// }

// static FILE * get_base_file(){
//     char * base_file_name = getenv(BASE_FILE);
//     char * aux = NULL;
//
//     if(base_file_name == NULL){
//         char * home_dir = getenv(HOME_DIR);
//         if(home_dir == NULL) return NULL;
//         asprintf(&aux, "%s/%s", home_dir, DEFAULT_BASE_FILE);
//         base_file_name = aux;
//     }
//
//     FILE * file = fopen(base_file_name, "r+");
//     if(file == NULL && errno == ENOENT)
//         file = fopen(base_file_name, "w+"); // create's the file
//                                             //
//     if(aux != NULL) free(aux);
//
//     return file;
// }
