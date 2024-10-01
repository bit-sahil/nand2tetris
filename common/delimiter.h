
#ifndef DELIMITER_H
#define DELIMITER_H


/* given a line/string, we usually want to search character/string
and also get both sides of strings delimited by that character

search is provided by strchr and strstr, both of which returns pointer to char/first char

init
first_half -- copy and return, without delim
second_half -- just give a pointer, without delim
next_word - copy and return, without delim

*/


typedef struct Delimiter {
    char* line;
    char* delim;
    int ignore_multiple_delim;

    char* ptr;
} Delimiter;


void advance_ptr(Delimiter* dl);
    // advance dl->ptr to point to beginning of next token

Delimiter* init_delimiter(char* line, char* delim, int ignore_multiple_delim);

int next_token(Delimiter* dl, char* dest);
    // copy next delimited token in dest, if present
    // returns True if word is found, false otherwise

#endif

