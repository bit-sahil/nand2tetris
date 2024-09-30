#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "boolean.h"
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


fConfig* init_file_config(char* file_name, int remove_space, int remove_trailing_space, int remove_comments, int non_empty_only, int max_len) {
    // initialize file config with settings and returns config

    fConfig* c = (fConfig*) malloc(sizeof(fConfig));
    strcpy(c->file_name, file_name);
    c->remove_space=remove_space;
    c->remove_trailing_space = remove_trailing_space;
    c->remove_comments = remove_comments;
    c->non_empty_only = non_empty_only;

    c->max_len = max_len;
    c->line = (char*) malloc(max_len*sizeof(char));

    c->fp = fopen(file_name, "r");
    c->state = CODE;

    return c;
}


void process_line(char* line, fConfig* c) {
    /* Processes line as per their current state and config
     stores result back in c->line
    */

    int idx=0;
    char curr_char;
    char cleaned_line[c->max_len];
    
    for(int i=0;line[i] != '\0';i++) {
        curr_char = line[i];

        if(c->state == COMMENT) {
            if((curr_char == '\n') || (curr_char == '\r')) {
                // different end of line conditions
                break;

            } else if(curr_char == '*' && line[i+1] == '/') {
                // end of comment
                i++;
                c->state=CODE;
                continue;
            }

        } else if(c->state==CHAR) {
            // only one way to terminate a char, and all characters are to be reported
            cleaned_line[idx++] = curr_char;

            if(curr_char=='\'' && line[i-1]!='\\') {
                // no escape sequence in char
                c->state=CODE;
            }

        } else if(c->state==STRING) {
            // only one way to terminate a string, and all characters are to be reported
            cleaned_line[idx++] = curr_char;

            if(curr_char=='"' && line[i-1]!='\\') {
                // string termination, '"' is already captured
                c->state=CODE;
            }

        } else if(c->state == CODE) {
            if((curr_char == '\n') || (curr_char == '\r')) {
                // different end of line conditions
                break;

            } else if(c->remove_comments && curr_char == '/' && line[i+1] == '/') {
                // if a slash is encountered, and next char is also a slash, then it's a simple end of line comment. Ignore rest of line
                break;

            } else if(c->remove_comments && curr_char == '/' && line[i+1] == '*') {
                // if a slash is encountered, and next char is '*', then everything that follows until */ is comment
                c->state=COMMENT;
                i++;
                continue;  // can break also, enforcing comment lines to not contain further code

            } else if((curr_char == ' ') || (curr_char == '\t')) {
                //remove all spaces from line
                if(c->remove_space)
                    continue;

                // remove space at beginning of line
                if(c->remove_trailing_space && idx==0)
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
                    c->state=CHAR;
                    continue;
                } else if(curr_char == '"') {
                    // inside string
                    c->state=STRING;
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

    strcpy(c->line, cleaned_line);
    //printf("IN:%s\n", cleaned_line);
    //printf("CIN:%s\n", c->line);
}


char* get_next_line(fConfig* c) {
    /* reads file at wherever current pointer is
    parse line as per config, and stores non-empty line in c->line,
    return pointer to line
    if there is no non-empty line, returns NULL pointer
    */

    char next_line[c->max_len];

    while(fgets(next_line, c->max_len, c->fp)) {
        process_line(next_line, c);

        if(c->non_empty_only &&  c->line[0] == '\0') {
            // only non-empty lines are to be reported
            continue;
        }

        //printf("IN2:%s\n", c->line);
        return c->line;
    }

    return NULL;
}


void dealloc_file_config(fConfig* c) {
    // deallocate memory assigned for config and it's sub-elements
    free(c->line);
    free(c);
}

