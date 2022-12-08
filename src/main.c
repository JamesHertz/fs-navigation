#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define HOME "HOME"
// where our little database will be stored
#define CONFIG_FILE_NAME ".fs-nav"

FILE * open_config_file(){
    char * home_dir = getenv(HOME);
    char * file_full_name; // config file full name

    asprintf(&file_full_name, "%s/%s", home_dir, CONFIG_FILE_NAME);

    printf("file-name: %s\n", file_full_name);

    FILE * file = fopen(file_full_name, "r");
    if(file == NULL){
        if(errno == ENOENT){
            printf("will create a new file :)\n");
            // TODO: create a new file
        }
        perror("");
    }

    // TODO: think if it's really worthed worring about the weird error :(    
    free(file_full_name);
    return file;
}

int main(void){

    FILE * f = open_config_file();
    if(f == NULL) {
        printf("Error could not open the file :(\n");
        return 1;
    }

    char data[256];
    int size;

    if((size = fread(data,1, 256, f)) < 0){
        printf("Error occured :(\n");
    }else{
        data[size] = '\0';
        printf("I read %s\n", data);
    }

    return 0;
}