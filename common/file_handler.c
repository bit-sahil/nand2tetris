#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include "boolean.h"
#include "file_handler.h"


/* Common code for handling files/directory

Takes in a file/directory name, with a in_extension, out_extension
If it's a file, then return file_name and FILE* to output file
If it's a directory, enumerates all *.in_extension files one by one
*/


dirConfig* init_dir_config(char* file_name, char* in_ext, char* out_ext) {
    // initialize config and returns dirConfig*
    
    dirConfig* c = (dirConfig*) malloc(sizeof(dirConfig));
    strcpy(c->file_name, file_name);
    strcpy(c->in_ext, in_ext);
    strcpy(c->out_ext, out_ext);
    c->file_sent = False;

    // whether it's directory or file with given extension
    char* p;
    if((p=strstr(file_name, in_ext)) != NULL) {
        // extension found in file_name
        // it's a file
        c->is_dir = False;
    } else {
        // it's a directory
        c->is_dir = True;
        if(!(c->dir=opendir(file_name))) {
            printf("Error while opening directory:%s\n", file_name);
        }
    }

    return c;
}


FILE* get_out_file(dirConfig* c) {
    // returns pointer to output file in "w" mode

    char file_name[128];
    strcpy(file_name, c->file_name);
    
    char* p;
    if((p=strstr(file_name, c->in_ext)) != NULL) {
        // file
        strcpy(p, c->out_ext);
    } else {
        strcat(file_name, c->out_ext);
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


int get_next_file(dirConfig* c, char* file_name) {
    // in case of file, return file_name only once

    if(!c->is_dir) {
        // file
        if(c->file_sent) {
            return False;
        }

        c->file_sent = True;
        strcpy(file_name, c->file_name);
        return True;
    }

    // handle directory
    struct dirent *d;
    char* p;
    
    while((d=readdir(c->dir)) != NULL) {
        // handle regular files only
        if(d->d_type != DT_REG)
            continue;

        if((p=strstr(file_name, c->in_ext)) != NULL) {
            // file with required extension
            get_file_name(c->file_name, d->d_name, file_name);
            // printf("//Processing file: %s\n", file_name);
            return True;
        }
    }

    return False;
}


void dealloc_dirconfig(dirConfig* c, FILE* fp) {
    // deallocate space taken by dirConfig and opened write file

    if(fp!=NULL)
        fclose(fp);

    if(c->is_dir)
        closedir(c->dir);

    free(c);
}

