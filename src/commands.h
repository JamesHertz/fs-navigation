#ifndef __commands__
#define __commands__

typedef void (*cmd_func) (int, char **);

typedef struct {
    char * name;
    cmd_func func;
} command;

typedef struct {
    command * commands;
    size_t count;
} commands;

cmd_func get_func(commands * cmds, char * c_name);


#endif