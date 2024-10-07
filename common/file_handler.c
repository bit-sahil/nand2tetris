#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<stdbool.h>
#include "file_handler.h"


/* Common code for handling files/directory

Takes in a file/directory name, with a in_extension, out_extension
If it's a file, then return file_name and FILE* to output file
If it's a directory, enumerates all *.in_extension files one by one
*/


DirConfig* init_dir_config(char* file_name, char* in_ext, char* out_ext) {
    // initialize config and returns DirConfig*
    
    DirConfig* dc = (DirConfig*) malloc(sizeof(DirConfig));
    strcpy(dc->file_name, file_name);
    strcpy(dc->in_ext, in_ext);
    strcpy(dc->out_ext, out_ext);
    dc->is_file_sent = false;

    // whether it's directory or file with given extension
    char* p;
    if((p=strstr(file_name, in_ext)) != NULL) {
        // extension found in file_name
        // it's a file
        dc->is_dir = false;
    } else {
        // it's a directory
        dc->is_dir = true;
        if(!(dc->dir=opendir(file_name))) {
            printf("Error while opening directory:%s\n", file_name);
        }
    }

    return dc;
}


FILE* get_out_file(DirConfig* dc, char* f_name) {
    // returns pointer to output file in "w" mode

    char file_name[128];
    strcpy(file_name, f_name);
    
    char* p;
    if((p=strstr(file_name, dc->in_ext)) != NULL) {
        // file
        strcpy(p, dc->out_ext);
    } else {
        // remove last '/' from dir path, if present
        int idx = 0;
        while(file_name[idx] != '\0')
            // move upto end of string
            idx++;

        if(file_name[idx-1] == '/')
            file_name[idx-1] = '\0';

        strcat(file_name, dc->out_ext);
    }

    return fopen(file_name, "w");
}


void get_file_name(char* dir_name, char* file_name, char* file_path) {
    // file_path = dir_name + '/' (optional) + file_name
    
    strcpy(file_path, dir_name);

    int idx = 0;
    while(file_path[idx] != '\0')
        // move upto end of string
        idx++;

    // append '/' if not present
    if(file_path[idx-1] != '/') {
        file_path[idx++] = '/';
        file_path[idx++] = '\0';
    }

    strcat(file_path, file_name);
}


int get_next_file(DirConfig* dc, char* file_name) {
    // in case of file, return file_name only once

    if(!dc->is_dir) {
        // file
        if(dc->is_file_sent) {
            return false;
        }

        dc->is_file_sent = true;
        strcpy(file_name, dc->file_name);
        return true;
    }

    // handle directory
    struct dirent *d;
    char* p;
    
    while((d=readdir(dc->dir)) != NULL) {
        // handle regular files only
        //printf("file in directory=%s;type=%d;in_ext=%s\n", d->d_name, d->d_type, dc->in_ext);

        if(d->d_type != DT_REG)
            continue;

        if((p=strstr(d->d_name, dc->in_ext)) != NULL) {
            // file with required extension
            get_file_name(dc->file_name, d->d_name, file_name);
            //printf("//Processing file: %s\n", file_name);
            return true;
        }
    }

    return false;
}


void dealloc_dir_config(DirConfig* dc, FILE* fp) {
    // deallocate space taken by DirConfig and opened write file

    if(fp!=NULL)
        fclose(fp);

    if(dc->is_dir)
        closedir(dc->dir);

    free(dc);
}

