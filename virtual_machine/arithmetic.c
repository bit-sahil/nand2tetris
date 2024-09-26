#include<stdio.h>
#include "cstring.h"
#include "arithmetic.h"


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


void handle_arithmetic_op(char* vc, FILE* asmFile) {
    // Handles arithmetic operations in a generic way
    // These operations do not contain any variables to deal with, so replacement from a static file work well

    char op_file[128];
    char f_contents[MAX_LINE_SIZE*MAX_LINES_HANDLED];
    get_op_file_name(vc, op_file);

    file_to_str(op_file, f_contents);

    // simply add contents of operation file to asmFile
    fputs(f_contents, asmFile);
}


void handle_comparison_op(char* vc, FILE* asmFile, int* line_num) {
    // Handles comparison operations in a generic way
    // These operations do not contain any variables as such, but we need jump statements with unique label
    // and use line_num to generate unique labels

    char op_file[128];
    char f_contents[MAX_LINE_SIZE*MAX_LINES_HANDLED];
    get_op_file_name(vc, op_file);

    file_to_str(op_file, f_contents);

    // simply add contents of operation file to asmFile
    fprintf(asmFile, f_contents, *line_num, *line_num, *line_num, *line_num);
}

