#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define HOME "HOME"
// where our little database will be stored
#define CONFIG_FILE_NAME ".fs-nav"

FILE * open_config_file(){
    char * home_dir = getenv(HOME);
    char * file_full_name = "file.txt"; // config file full name

    printf("file-name: %s\n", file_full_name);

    FILE * file = fopen(file_full_name, "r+");
    if(file == NULL && errno == ENOENT)
        file = fopen(file_full_name, "w+"); // create's the file
    

    // TODO: think if it's really worthed worring about the weird error :(    
    // free(file_full_name);
    return file;
}

int main(int argc, char * argv[]){

    char * cmd = NULL;
    char ** args = NULL;
    int argscount = 0;

    for(int i = 1; i < argc; i++){
        if(i == 1) cmd = argv[i];
        else if (i == 2){
            args = &argv[i];
            argscount = argc - i; // new argc
            break;
        } 
    }

    printf("cmd: %s\n", cmd);
    printf("args: ");
    if(args == NULL)
        printf("(null)\n");
    else{
        for(int i = 0; args[i]; i++){
            printf("%s, ", args[i]);
        }
        printf("\n");
    }
    printf("argscount: %d\n", argscount);


//    FILE * f = open_config_file();
//    if(f == NULL) {
//        printf("Error could not open the file :(\n");
//        return 1;
//    }
//
//    char data[256];
//    int size;
//
//    if((size = fread(data,1, 256, f)) < 0){
//        printf("Error occured :(\n");
//    }else{
//        data[size] = '\0';
//        printf("I read %s\n", data);
//    }

//    fclose(f);
    return 0;
}