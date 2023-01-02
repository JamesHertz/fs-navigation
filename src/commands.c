#include <stdio.h>
#include <string.h>
#include "commands.h"

cmd_func get_func(commands * cmds, char * c_name){
 
     for(int i = 0; i < cmds->count; i++){
         command * cmd = &cmds->commands[i];
         if(!strcmp(cmd->name, c_name)) return cmd->func;
     }
 
     return NULL;
 }