#ifndef __RECORD__
#define __RECORD__

#include <stdbool.h>

typedef struct {
    char * name;
    char * path;
} record;

typedef struct _lnode{
    record record;
    struct _lnode* next;
} lnode;

typedef struct {
    lnode * head;
    lnode * tail;
    size_t size;
} llist;

typedef struct {
    llist  records;
    FILE * storage;
} RecordsManager;

RecordsManager * load_records();

void save_records(RecordsManager * m);

record * get_record(const RecordsManager * m, char * name);

char * remove_record(RecordsManager * m, char * name);

char * create_record(RecordsManager * m, char * name, char * path);

void close_storage(RecordsManager * m);

#endif