#include<stdio.h>
#include<stdlib.h>
#include "cstring.h"
#include "helper.h"


void get_op_file_name(char* vc, char* op_file) {
    // return .asm file path

    replace_substr_end(op_file, "op/", 0);
    replace_substr_end(op_file, vc, -1);
    replace_substr_end(op_file, ".asm", -1);
}


void file_to_str(char* file_name, char* f_contents) {
    // open file and store it's contents into f_contents string
    // assume f_contents has sufficient space and there is no overflow

    FILE* fp = fopen(file_name, "r");
    f_contents[0] = '\0'; // start with an empty string

    char line[MAX_LINE_SIZE] = {0};
    while(fgets(line, MAX_LINE_SIZE, fp)) {
        replace_substr_end(f_contents, line, -1);
    }

    fclose(fp);
}


char* get_f_contents(char* vc) {
    // takes virtual command to be found in op/ folder and return it's file contents as a string

    char op_file[128];
    char* f_contents = (char*) malloc(MAX_LINE_SIZE*MAX_LINES_HANDLED*sizeof(char));
    get_op_file_name(vc, op_file);

    file_to_str(op_file, f_contents);
    return f_contents;
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

