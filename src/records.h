#ifndef __RECORD__
#define __RECORD__

#include <stdbool.h>

typedef struct __record__ {
    char * name;
    char * path;
} * record;

typedef struct __lnode * lnode;

record get_record(char * name);
void create_record(char * name, char * path);

// iterator for the records
lnode get_records_iterator();

bool it_has_next(lnode node);

// prec: node != NULL
lnode it_next (lnode node);

// prec: node != NULL
record it_node_record(lnode node);

#endif