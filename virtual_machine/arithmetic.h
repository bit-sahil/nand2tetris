
#ifndef ARITHMETIC_H
#define ARITHMETIC_H


void handle_arithmetic_op(char* vc, FILE* asmFile);
    // Handles arithmetic operations in a generic way
    // These operations do not contain any variables to deal with, so replacement from a static file work well

void handle_comparison_op(char* vc, FILE* asmFile, int* line_num);
    // Handles comparison operations in a generic way
    // These operations do not contain any variables as such, but we need jump statements with unique label
    // and use line_num to generate unique labels

#endif
