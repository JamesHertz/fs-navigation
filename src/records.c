#include "records.h"
#include <stdio.h>
#include <errno.h>

#define HOME "HOME"
// where our little database will be stored
#define CONFIG_FILE_NAME ".fs-nav"

static FILE * config_file;


// when the file it's open don't open it again :)
static FILE * open_config_file(){
    char * home_dir = getenv(HOME);
    char * file_full_name = "file.txt"; // config file full name

    //asprintf(&file_full_name, "%s/%s", home_dir, CONFIG_FILE_NAME);

    printf("file-name: %s\n", file_full_name);

    FILE * file = fopen(file_full_name, "r+");
    if(file == NULL && errno == ENOENT)
        file = fopen(file_full_name, "w+"); // create's the file
    
    // TODO: think if it's really worthed worring about the weird error :(    
    // free(file_full_name);
    return file;
}

record get_record(char * name){
    return NULL;
}


void create_record(char * name, char * path){
    // do something
}
