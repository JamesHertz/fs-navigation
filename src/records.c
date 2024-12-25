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

#define HOME_DIR "HOME"
#define INITIAL_RECORDS_SIZE 64

static void * realloc_memory(void * old, size_t size) {
    void * buffer = realloc(old, size);
    if(buffer == NULL) {
        fprintf(stderr, "[ ERROR ] Failed to (re)allocate %zu bytes (You probably ran out of memory)!\n", size);
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
    for(size_t i = 0; i < m->records.length; i++){
        Record * rec = &m->records.items[i];
        if(strcmp(rec->name, name) == 0)
            return i;
    }
    return NOT_FOUND;
}

static Record * rm_find_record_entry(const RecordsManager * m, const char * name){
    ssize_t idx = rm_find_record_index(m, name);
    return idx == NOT_FOUND ? NULL : &m->records.items[idx];
}

char * resolve_base_filename() {
    char * filename = getenv(BASE_FILE_ENV_VAR);
    if(filename == NULL) {
        char * home_dir = getenv(HOME_DIR);
        assert(home_dir != NULL && "'HOME' env var no defined");
        asprintf(&filename, "%s/%s", home_dir, DEFAULT_BASE_FILE);
        return filename;
    }
    return strdup(filename);
}

RecordsManager * rm_load_records(const char * base_filename) {
    struct stat st;
    int fd = open(base_filename, O_RDWR | O_CREAT, 0666);

    if(fd < 0 || fstat(fd, &st) != 0) 
        return NULL;

    FILE * storage = fdopen(fd, "a+");
    assert(storage != NULL);

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

char * rm_put_record(RecordsManager * m, const char * name, const char * path) {
    // TODO: think about optimizations when name == path
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
    return (char *) old_path;
}

char * rm_remove_record(RecordsManager * m, const char * name) {
    ssize_t idx = rm_find_record_index(m, name);
    if(idx == NOT_FOUND) return NULL;

    m->dirty = true;

    Record rec = m->records.items[idx];
    size_t last_idx = m->records.length - 1;
    if(last_idx >= 0) {
        m->records.items[idx]      = m->records.items[last_idx];
        m->records.items[last_idx] = (Record) {0};
    }

    m->records.length--;
    free((void *) rec.name);
    return (char *) rec.path;
}

int rm_destroy(RecordsManager * m) {
    if(m->dirty) {
        fseek(m->storage, 0, SEEK_SET);
        ftruncate(fileno(m->storage), 0); // truncates the file

        RM_FOR_EACH_RECORD(m, rec, {
            fprintf(m->storage, REC_LINE_FORMAT, rec.name, rec.path);
        });
        fflush(m->storage);
    }

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
