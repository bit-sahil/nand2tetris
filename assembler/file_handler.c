#include<stdio.h>
#include<stdlib.h>
#include<string.h>


/* 
I think instead of trying to write most readable code, it might be good to also try most optimized code 
cos assemblers back in the day did not have luxury of large memories.
So instead of modular functionality/data processing pipeline type behavior, I could try to optimize wrt resources (memory and time)
But not everywhere, at some places I'd still relax under luxury of too many resources.

I would not, though, try to work without stdio library. Might get off libraries like stdbool and use simple integers (less optimized now!!)
But, assembly language I just worked with, for which we just made a CPU chip, memory chip and a working computer wouldn't have luxury of boolean bits,
so perhaps not too bad
*/


#define MAX_LINE_SIZE 128


void print_line(char* line) {
    // just prints a line. %s was printing random characters so resorted to a basic function
    int idx = 0;
    while(1) {
        printf("%c", line[idx]);
        if((line[idx] == '\n') || (line[idx] == '\0')) {
            break;
        }
        idx++;
    }
    
    printf("\n");
}


int print_instruction_with_line_number(char* line, int* current_line_number, int print) {
    // print line with line number, ignoring empty lines if print=1, print=0 for skip printing
    // increments line number for non-empty lines
    // returns whether line is and instruction by 1/0

    if(line[0] == '\0') {
        //empty line
        return 0;
    }

    if(print) {
        printf("%d: ", *current_line_number);
        print_line(line);
    }

    *current_line_number = *current_line_number + 1;
    return 1;
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
            if(idx==0) { 
                continue; //ignore all spaces in beginning of line
            }

            // ignore whitespace and tabs. Convert tab to whitespace in new line, and collapse all, in case there is such a sequence
            if ((idx>0) && (cleaned_line[idx-1] == ' ')) {
                continue; // there's already a whitespace, ignore another
            }
            cleaned_line[idx] = ' ';
            idx++;

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


void read_file_and_process_instructions(char* file_name) {
    /* This function opens a file, and process it's line one by one, ignoring comments and whitespaces
    Then we want line numbers (as addresses of given instruction, for jump instruction to be functional, and that's what we do
    
    This function could very well return a list of instructions, and that's make life much easier, 
    but that seems to assume more luxury of RAM space than I want to allow.
    So, I try to process each line one by one. 
    This would have to duplicate the effort required to read, clean, and number lines. 
    And currently (I might be wrong), this effort seems lesser compared to having to store instructions.
    */
    
    FILE *asmFile = fopen(file_name, "r"); //.asm file

    char line[MAX_LINE_SIZE] = {0};
    char cleaned_line[MAX_LINE_SIZE] = {0};
    int current_line_number = 0;
    int is_instruction = 0;

    while(fgets(line, MAX_LINE_SIZE, asmFile)) {
        line_without_space(line, cleaned_line);
        // printf("ORIGINAL: %s", line);
        is_instruction = print_instruction_with_line_number(cleaned_line, &current_line_number, 1);
    }
}


int main(){
    read_file_and_process_instructions("Add.asm");
    return 0;
}

