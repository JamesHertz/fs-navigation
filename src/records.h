#ifndef __RECORD__
#define __RECORD__

typedef struct __record__ {
    char * name;
    char * path;
} * record;

record get_record(char * name);
void create_record(char * name, char * path);

void print_records();
#endif