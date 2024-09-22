#include<stdio.h>
#include<stdlib.h>
#include "cstring.h"
#include "inst_helper.h"


int instruction_type_with_line_number(char* line, int* current_line_number, int print) {
    // print line with line number, ignoring empty lines if print=1, print=0 for skip printing
    // increments line number for non-empty lines excluding Label symbols
    // returns whether line is empty, label, or normal machine instruction

    if(line[0] == '\0') {
        //empty line
        return Empty;
    }
    
    if(line[0] == '(')
        return Label;


    if(line[0] == '$') {
        //this is a macro
        return Macro;
    }

    *current_line_number = *current_line_number + 1;

    if(print) {
        printf("%d: ", *current_line_number);
        print_str(line);
        printf("\n");
    }

    return Machine;
}


void line_without_space(char* line, char* cleaned_line) {
    /* Input a line from assembly program, and remove all whitespaces, comments, and tab
    tab is converted to whitespace, wherever delimiter is required
    Converted line is stored in cleaned_line variable
    */

    int idx = 0;
    char curr_char;
    
    for(int i=0;i<MAX_LINE_SIZE;i++) {
        curr_char = line[i];
        
        if((curr_char == '\n') || (curr_char == '\0') || (curr_char == '\r')) {
            // end of line
            // '\0' is perhaps not required for reading file, cos '\n' should always terminate all lines, except possibly last
            // added this to avoid unexpected failured in future
            break;

        } else if(curr_char == '/' && line[i+1] == '/') {
            // if a slash is encountered, and next char is also a slash, then it's a comment. Ignore rest of line
            break;

        } else if((curr_char == ' ') || (curr_char == '\t')) {
            // no space is required in assembly language
            continue;

        } else {
            // else it's a character relevant to assembly language
            // printf("Relevant char: %c | %x \n", curr_char, curr_char);
            cleaned_line[idx] = curr_char;
            idx++;
        }
    }
    
    // mark end of string to avoid reading garbage
    cleaned_line[idx] = '\0';
}

