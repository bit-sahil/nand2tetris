#include<stdio.h>
#include<stdlib.h>
#include "helper.h"
#include "arithmetic.h"


void handle_arithmetic_op(char* vc, FILE* asmFile) {
    // Handles arithmetic operations in a generic way
    // These operations do not contain any variables to deal with, so replacement from a static file work well

    char* f_contents = get_f_contents(vc);

    // simply add contents of operation file to asmFile
    fputs(f_contents, asmFile);

    free(f_contents);
}


void handle_comparison_op(char* vc, FILE* asmFile, int line_num, char* fvar) {
    // Handles comparison operations in a generic way
    // These operations do not contain any variables as such, but we need jump statements with unique label
    // and use line_num to generate unique labels

    char* f_contents = get_f_contents(vc);

    // add formatted contents of operation file to asmFile
    fprintf(asmFile, f_contents, fvar, line_num, fvar, line_num, fvar, line_num, fvar, line_num);

    free(f_contents);
}


