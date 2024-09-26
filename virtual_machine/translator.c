#include<stdio.h>
#include "cstring.h"
#include "writer.h"


/* This is entry point into virtual machine translator
Takes in a f.vm file and outputs f.asm file post translation
Instead of trying to re-use elements from compiler/ we'd just copy those over,
unless it should be a utility, in which case makefile would do well

*/


#define MAX_LINE_SIZE 128


void get_asm_file_name(char* file_name, char* asm_file_name) {
    // replace .vm with .asm and store value in asm_file_name
    
    copy_str(asm_file_name, file_name);
    int idx = find_substr(asm_file_name, ".vm");
    replace_substr_end(asm_file_name, ".asm", idx);
}


void line_without_space(char* line, char* cleaned_line) {
    /* Input a line from virtual machine program, and remove all whitespaces, comments, and tab
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
            // no space is required at beginning of line
            if(idx==0)
                continue;

            // ignore double spaces and convert tab to single space
            if(idx>0 && cleaned_line[idx-1]!=' ') {
                cleaned_line[idx] = ' ';
                idx++;
            }

        } else {
            // else it's a character relevant to virtual machine language
            // printf("Relevant char: %c | %x \n", curr_char, curr_char);
            cleaned_line[idx] = curr_char;
            idx++;
        }
    }

    // remove trailing spaces, if any
    if(idx>0 && cleaned_line[idx-1]==' ')
        idx--;
    
    // mark end of string to avoid reading garbage
    cleaned_line[idx] = '\0';
}


void process_file_and_translate(char* file_name) {
    /* This function opens .vm file for reading and parse it line by line, ignoring whitespaces and comments,
    and write generated assembly language code in .asm file with same name

    First tast is reading file line by line and cleaning it out of spaces and comments. Line number is not required.
    Then we parse to figure out type of command, and based on that subtype and arg, if applicable
    Finally handle command and generate assembly code
    */

    FILE *vmFile = fopen(file_name, "r"); //.vm file
    
    char asm_file_name[128];
    get_asm_file_name(file_name, asm_file_name);
    FILE *asmFile = fopen(asm_file_name, "w"); //.asm file created in same path as .vm file

    char line[MAX_LINE_SIZE] = {0};
    char cleaned_line[MAX_LINE_SIZE] = {0};
    int line_num = 0;

    while(fgets(line, MAX_LINE_SIZE, vmFile)) {
        line_without_space(line, cleaned_line);
        // printf("Original:%s;Cleaned:%s\n", line, cleaned_line);

        parse_and_generate_asm(cleaned_line, asmFile, &line_num);
    }

    fclose(vmFile);
    fclose(asmFile);
}


int main(int argc, char* argv[]) {
    // vm file is provided as argv[1]

    char* file_name = argv[1];

    process_file_and_translate(file_name);
}

