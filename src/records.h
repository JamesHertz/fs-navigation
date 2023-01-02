#ifndef __RECORD__
#define __RECORD__

#include <stdbool.h>

typedef struct {
    char * name;
    char * path;
} record;

typedef struct _llist {
    record record;
    struct _llist * next;
} llist;

typedef struct {
    llist * records;
    FILE * storage;
} RecordsManager;

RecordsManager * load_records();

record * get_record(RecordsManager * m, char * name);

void create_record(RecordsManager * m, char * name, char * path);

#endif