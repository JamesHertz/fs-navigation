#ifndef __commands__
#define __commands__

typedef void (*cmd_func) (int, char **);

typedef struct {
    char * name;
    cmd_func func;
} command;

cmd_func get_func(char * c_name);

#endif