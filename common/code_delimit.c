#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include "code_state.h"
#include "delimiter.h"
#include "word_delimit.h"


Delimiter* init_code_delimiter(char* line) {
    //printf("Allocating delimiter with line:%s\n", line);

    Delimiter* dl = init_word_delimiter(line);
    dl->state = CODE;

    return dl;
}


int next_code_token(Delimiter* dl, char* dest) {
    /* copy next delimited token in dest, if present
    returns true if word is found, false otherwise

    Since we're parsing code, we need to preserve string and char as it is
    For code"str"code, we don't delimit wrt delimiter inside str, and use beginning and end of string
    as natural delimiters irrespective of delimiter provided
    */

    if(dl->ptr == NULL)
        return false;

    char* line = dl->ptr;
    int sz = strlen(dl->delim);

    int i=0;
    char curr_char;

    // printf("line=%s;delim=%s;sz=%d\n", line, dl->delim, sz);
    
    for( ; line[i] != '\0' ; i++) {
        curr_char = line[i];
        // printf("curr_char=%c;\n", curr_char);

        if(dl->state==CHAR) {
            // only one way to terminate a char, and all characters are to be reported
            dest[i] = curr_char;

            if(curr_char=='\'' && line[i-1]!='\\') {
                // no escape sequence in char
                // terminate char
                dl->state=CODE;
                i++;
                break;
            }

        } else if(dl->state==STRING) {
            // only one way to terminate a string, and all characters are to be reported
            dest[i] = curr_char;

            if(curr_char=='"' && line[i-1]!='\\') {
                // string termination
                // printf("string termination:%c:%c:\n", curr_char, line[i-1]);
                dl->state=CODE;
                i++;
                break;
            }

        } else if(dl->state == CODE) {
            if((curr_char == '\n') || (curr_char == '\r')) {
                // different end of line conditions
                // we shouldn't ideally reach here
                printf("Error: end of line in code line: %s\n", line);
                break;

            } else {
                if(strncmp(dl->delim, &line[i], sz) == 0) {
                    // found delimiter -- break from here
                    break;

                } else if(curr_char == '\'') {
                    if(i==0) {
                        // beginning char
                        dl->state=CHAR;
                        dest[i] = curr_char;
                        continue;
                    } else {
                        // end of code, next token shall be beginning of char
                        break;
                    }

                } else if(curr_char == '"') {
                    if(i==0) {
                        // beginning string
                        dl->state=STRING;
                        dest[i] = curr_char;
                        continue;
                    } else {
                        // end of code, next token shall be beginning of char
                        break;
                    }

                } else {
                    // normal code character to be added
                    dest[i] = curr_char;
                }
            }
        }
    }

    dl->ptr = dl->ptr + i;
    dest[i] = '\0';

    advance_ptr(dl);

    return true;
}

