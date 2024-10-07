#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "code_state.h"
#include "file_reader.h"


/* Reads the input file, line by line, and returns next non-empty line based on config

Config options:
    no_space: Remove all spaces
    no_trailing_space: Remove all spaces at beginning and end of line
    no_comments: Remove all comments starting with //, and all block comments

We can provide more config options but this seems enough for current use-case.
We always replace \t with ' ', and replace multiple of such occurances with single ' '

We model this using states in which current cursor could be,
Comment: after encountering / *.. until * /
Code: default state
Inside_String: (or char) When we're inside '' or "" where comment condition needs to be ignored
Inside string escape sequences? - handle \" or \' if you're inside string

To keep things simpler, we can provide FILE* as input instead of file_name,
for not having to store FILE* as part of moving config back and forth
*/


ReaderConfig* init_reader_config(char* file_name, int remove_space, int remove_trailing_space, int remove_comments, int non_empty_only, int max_len) {
    // initialize file config with settings and returns config

    ReaderConfig* rc = (ReaderConfig*) malloc(sizeof(ReaderConfig));
    strcpy(rc->file_name, file_name);
    rc->remove_space=remove_space;
    rc->remove_trailing_space = remove_trailing_space;
    rc->remove_comments = remove_comments;
    rc->non_empty_only = non_empty_only;

    rc->max_len = max_len;
    rc->line = (char*) malloc(max_len*sizeof(char));

    rc->fp = fopen(file_name, "r");
    rc->state = CODE;

    return rc;
}


void process_line(char* line, ReaderConfig* rc) {
    /* Processes line as per their current state and config
     stores result back in rc->line
    */

    int idx=0;
    char curr_char;
    char cleaned_line[rc->max_len];
    
    for(int i=0;line[i] != '\0';i++) {
        curr_char = line[i];

        if(rc->state == COMMENT) {
            if((curr_char == '\n') || (curr_char == '\r')) {
                // different end of line conditions
                break;

            } else if(curr_char == '*' && line[i+1] == '/') {
                // end of comment
                i++;
                rc->state=CODE;
                continue;
            }

        } else if(rc->state==CHAR) {
            // only one way to terminate a char, and all characters are to be reported
            cleaned_line[idx++] = curr_char;

            if(curr_char=='\'' && line[i-1]!='\\') {
                // no escape sequence in char
                rc->state=CODE;
            }

        } else if(rc->state==STRING) {
            // only one way to terminate a string, and all characters are to be reported
            cleaned_line[idx++] = curr_char;

            if(curr_char=='"' && line[i-1]!='\\') {
                // string termination, '"' is already captured
                rc->state=CODE;
            }

        } else if(rc->state == CODE) {
            if((curr_char == '\n') || (curr_char == '\r')) {
                // different end of line conditions
                break;

            } else if(rc->remove_comments && curr_char == '/' && line[i+1] == '/') {
                // if a slash is encountered, and next char is also a slash, then it's a simple end of line comment. Ignore rest of line
                break;

            } else if(rc->remove_comments && curr_char == '/' && line[i+1] == '*') {
                // if a slash is encountered, and next char is '*', then everything that follows until */ is comment
                rc->state=COMMENT;
                i++;
                continue;  // can break also, enforcing comment lines to not contain further code

            } else if((curr_char == ' ') || (curr_char == '\t')) {
                //remove all spaces from line
                if(rc->remove_space)
                    continue;

                // remove space at beginning of line
                if(rc->remove_trailing_space && idx==0)
                    continue;

                // ignore double spaces and convert tab to single space
                if(idx>0 && cleaned_line[idx-1]!=' ') {
                    cleaned_line[idx] = ' ';
                    idx++;
                }

            } else {
                // it's a relevant code character
                // printf("Relevant char: %c | %x \n", curr_char, curr_char);
                cleaned_line[idx++] = curr_char;

                if(curr_char == '\'') {
                    // inside char
                    rc->state=CHAR;
                    continue;
                } else if(curr_char == '"') {
                    // inside string
                    rc->state=STRING;
                    continue;
                }
            }
        }
    }

    // remove trailing spaces, if any
    if(idx>0 && cleaned_line[idx-1]==' ')
        idx--;
    
    // mark end of string to avoid reading garbage
    cleaned_line[idx] = '\0';

    strcpy(rc->line, cleaned_line);
    //printf("IN:%s\n", cleaned_line);
    //printf("CIN:%s\n", rc->line);
}


char* get_next_line(ReaderConfig* rc) {
    /* reads file at wherever current pointer is
    parse line as per config, and stores non-empty line in rc->line,
    return pointer to line
    if there is no non-empty line, returns NULL pointer
    */

    char next_line[rc->max_len];

    while(fgets(next_line, rc->max_len, rc->fp)) {
        process_line(next_line, rc);

        if(rc->non_empty_only &&  rc->line[0] == '\0') {
            // only non-empty lines are to be reported
            continue;
        }

        //printf("IN2:%s\n", rc->line);
        return rc->line;
    }

    return NULL;
}


void dealloc_reader_config(ReaderConfig* rc) {
    // deallocate memory assigned for config and it's sub-elements
    free(rc->line);
    free(rc);
}

