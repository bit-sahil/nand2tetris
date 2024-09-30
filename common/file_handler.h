#include<stdio.h>
#include<dirent.h>


#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

typedef struct dirConfig {
    char file_name[128];
    char in_ext[128];
    char out_ext[128];

    int is_dir;
    int file_sent;
    DIR* dir;
} dirConfig;

dirConfig* init_dir_config(char* file_name, char* in_ext, char* out_ext);
    // initialize config and returns dirConfig*
    
FILE* get_out_file(dirConfig* c);
    // returns pointer to output file in "w" mode

int get_next_file(dirConfig* c, char* file_name);
    // in case of file, return file_name only once

void dealloc_dirconfig(dirConfig* c, FILE* fp);
    // deallocate space taken by dirConfig and opened write file

#endif

