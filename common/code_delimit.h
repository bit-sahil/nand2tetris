#include "delimiter.h"


#ifndef CODE_DELIMITER_H
#define CODE_DELIMITER_H


Delimiter* init_code_delimiter(char* line);

int next_code_token(Delimiter* dl, char* dest);
    /* copy next delimited token in dest, if present
    returns True if word is found, false otherwise

    Since we're parsing code, we need to preserve string and char as it is
    For code"str"code, we don't delimit wrt delimiter inside str, and use beginning and end of string
    as natural delimiters irrespective of delimiter provided
    */


#endif

