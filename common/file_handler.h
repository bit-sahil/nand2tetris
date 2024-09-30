#include<stdio.h>
#include<dirent.h>


#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

typedef struct DirConfig {
    char file_name[128];
    char in_ext[128];
    char out_ext[128];

    int is_dir;
    int is_file_sent;
    DIR* dir;
} DirConfig;

DirConfig* init_dir_config(char* file_name, char* in_ext, char* out_ext);
    // initialize config and returns DirConfig*
    
FILE* get_out_file(DirConfig* dc, char* f_name);
    // returns pointer to output file in "w" mode

int get_next_file(DirConfig* dc, char* file_name);
    // in case of file, return file_name only once

void dealloc_dir_config(DirConfig* dc, FILE* fp);
    // deallocate space taken by DirConfig and opened write file

#endif

