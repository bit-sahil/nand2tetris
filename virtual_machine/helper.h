
#ifndef HELPER_H
#define HELPER_H

#define MAX_LINE_SIZE 128
#define MAX_LINES_HANDLED 64


char* get_f_contents(char* vc);
    // takes virtual command to be found in op/ folder and return it's file contents as a string

void get_file_variable_name(char* file_name, char* fvar);
    // takes in full file name, i.e. a/b/c.ext and returns c
    // apart from initialization in this function, fvar is treated as a state variable and it's updated
    // when we enter or exit a function by function and return handler respectively

#endif

