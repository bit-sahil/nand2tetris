#include<stdio.h>
#include<stdlib.h>
#include "cstring.h"
#include "helper.h"
#include "../common/file_content.h"


char* get_f_contents(char* vc) {
    // takes virtual command to be found in op/ folder and return it's file contents as a string

    char op_file[128];
    op_file_name(vc, "op/", ".asm", op_file);

    return file_to_str(op_file, MAX_LINE_SIZE, MAX_LINES_HANDLED);
}


void get_file_variable_name(char* file_name, char* fvar) {
    // takes in full file name, i.e. a/b/c.ext and returns c
    // apart from initialization in this function, fvar is treated as a state variable and it's updated
    // when we enter or exit a function by function and return handler respectively

    char temp[128];
    copy_str(temp, file_name);

    int s_idx = 0;

    // remove all '/'
    int idx = search_char(temp, '/');
    while(idx!=-1) {
        s_idx += idx+1;
        idx = search_char(&temp[s_idx], '/');
        //printf("%d:%d:%s\n", idx, s_idx, &temp[idx+1]);
    }

    int max = search_char(&temp[s_idx], '.');
    copy_str_until(fvar, &temp[s_idx], max);
}

