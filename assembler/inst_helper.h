
#ifndef INST_HELPER_H
#define INST_HELPER_H

#define MAX_LINE_SIZE 128
#define Empty 0
#define Label 1
#define Machine 2
#define Macro 3


int instruction_type_with_line_number(char* line, int* current_line_number, int print); 
    /* print line with line number, ignoring empty lines if print=1, print=0 for skip printing
    increments line number for non-empty lines excluding Label symbols
    returns whether line is empty, label, or normal machine instruction
    */


void line_without_space(char* line, char* cleaned_line);
    /* Input a line from assembly program, and remove all whitespaces, comments, and tab
    tab is converted to whitespace, wherever delimiter is required
    Converted line is stored in cleaned_line variable
    */


#endif
