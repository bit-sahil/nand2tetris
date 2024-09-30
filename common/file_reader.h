#include<stdio.h>


#ifndef FILE_READER_H
#define FILE_READER_H


typedef enum {
    CODE = 1,
    COMMENT = 2,
    CHAR = 3,
    STRING = 4,
} CodeState;


typedef struct ReaderConfig {
    char file_name[128];
    int remove_space;
    int remove_trailing_space;
    int remove_comments;
    int non_empty_only;

    int max_len;
    char* line;

    CodeState state;
    FILE* fp;
} ReaderConfig;

ReaderConfig* init_file_config(char* file_name, int remove_space, int remove_trailing_space, int remove_comments, int non_empty_only, int max_len);
    // initialize file config with settings and returns config


char* get_next_line(ReaderConfig* rc);
    /* reads file at wherever current pointer is
    parse line as per config, and stores non-empty line in c->line,
    return pointer to line
    if there is no non-empty line, returns NULL pointer
    */

void dealloc_file_config(ReaderConfig* rc); 
    // deallocate memory assigned for config and it's sub-elements

#endif

