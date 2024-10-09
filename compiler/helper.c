#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "helper.h"
#include "../common/file_content.h"


char* get_f_contents(char* command) {
    // takes jack language construct type to be found in op/ folder 
    // and return it's file contents as a string

    char op_file[128];
    op_file_name(command, "op/", ".vm", op_file);

    return file_to_str(op_file, MAX_LINE_SIZE, MAX_LINES_HANDLED);
}
