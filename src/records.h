#ifndef __RECORD__
#define __RECORD__

#include <stdbool.h>
#include <stdio.h>

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
        for(int i = 0; i < m->records.length; i++){    \
            Record record_name = m->records.items[i];  \
            body                                       \
        }                                              \
    } while(0)

#endif
