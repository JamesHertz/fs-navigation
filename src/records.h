#ifndef __RECORD__
#define __RECORD__

#include <stdbool.h>
#include <stdio.h>

#define BASE_FILE_ENV_VAR "FS_BASE_FILE"
#define DEFAULT_BASE_FILE ".fs-nav"

typedef struct {
    const char * name;
    const char * path;
} Record;

typedef struct {
    struct {
        size_t capacity;
        size_t length;
        Record * items;
    } records;
    FILE * storage;
    bool dirty;
} RecordsManager;


// Tries to get base_filename from BASE_FILE_ENV_VAR if such is not defined. If not it will assume
// the file to be "$HOME/" + DEFAULT_BASE_FILE
// @returns an heap allocated pointer that must be freed
char * resolve_base_filename();

// Opens `base_filename` (creates it if it doesn't exit) and loads its contents, 
// in case of failure the appropriated errno value is set.
// @returns a pointer to a new initialized RecordManager
RecordsManager * rm_load_records(const char * base_filename);

// @return a owned pointer (must be freed) to the `path` that was previously associatd with `name`
//      or NULL if there was none
char * rm_put_record(RecordsManager * m, const char * name, const char * path);

// @return a owned pointer (must be freed) to the `path` that was previously associatd with `name`
//      or NULL if there was none
char * rm_remove_record(RecordsManager * m, const char * name);

// @returns the path associated with `name` or NULL if there is none, the pointer should no be freed
const char * rm_find_path(const RecordsManager * m, const char * name);

// @returns: 0 for ok and something else for error with the appropriated errno value is set
int rm_destroy(RecordsManager * m);

static inline size_t rm_records_number(const RecordsManager * m){
    return m->records.length;
}

#define RM_FOR_EACH_RECORD(m, record_name, body) do {  \
        for(int __i = 0; __i < m->records.length; __i++){    \
            Record record_name = m->records.items[__i];  \
            body                                       \
        }                                              \
    } while(0)

#endif
