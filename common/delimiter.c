#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include "delimiter.h"


/* given a line/string, we usually want to search character/string
and also get both sides of strings delimited by that character

search is provided by strchr and strstr, both of which returns pointer to char/first char

init
first_half -- copy and return, without delim
second_half -- just give a pointer, without delim
next_word - copy and return, without delim

*/


void advance_ptr(Delimiter* dl) {
    // advance dl->ptr to point to beginning of next token

    int sz = strlen(dl->delim);
    //printf("sz=%d\n", sz);
    if(!dl->ignore_multiple_delim)
        return;

    // look for multiple delims
    while(strncmp(dl->delim, dl->ptr, sz) == 0) {
        // keep advancing ptr by sz
        dl->ptr = dl->ptr + sz;
    }

    if(dl->ptr[0] == '\0')
        dl->ptr = NULL;
}


Delimiter* init_delimiter(char* line, char* delim, int ignore_multiple_delim) {
    //printf("Allocating delimiter with line:%s\n", line);

    Delimiter* dl = (Delimiter*) malloc(sizeof(Delimiter));

    dl->line = (char*) malloc((1+strlen(line))*sizeof(char));
    //dl->line = (char*) malloc(128*sizeof(char));
    //dl->delim = (char*) malloc(128*sizeof(char));
    dl->delim = (char*) malloc((1+strlen(delim))*sizeof(char));
    dl->ignore_multiple_delim = ignore_multiple_delim;

    strcpy(dl->line, line);
    strcpy(dl->delim, delim);
    dl->ptr = dl->line;
    advance_ptr(dl);

    return dl;
}


int next_token(Delimiter* dl, char* dest) {
    // copy next delimited token in dest, if present
    // returns true if word is found, false otherwise

    if(dl->ptr == NULL)
        return false;

    char* p = strstr(dl->ptr, dl->delim);

    if(p==NULL) {
        strcpy(dest, dl->ptr);
        dl->ptr = NULL;
        return true;
    }

    //printf("p:%s\n", p);

    int i=0;
    for(;&dl->ptr[i] != p;i++)
        dest[i] = dl->ptr[i];

    dest[i] = '\0';

    // advance ptr
    dl->ptr = p + strlen(dl->delim);
    advance_ptr(dl);

    return true;
}


void dealloc_delimiter(Delimiter* dl) {
    
    //printf("Deallocating delimiter for line: %s\n", dl->line);

    free(dl->line);
    free(dl->delim);
    free(dl);
}

