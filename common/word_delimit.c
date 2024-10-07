#include<string.h>
#include<stdbool.h>
#include "word_delimit.h"
#include "delimiter.h"


/* given a line/string, we usually want to search character/string
and also get both sides of strings delimited by that character

search is provided by strchr and strstr, both of which returns pointer to char/first char

init
first_half -- copy and return, without delim
second_half -- just give a pointer, without delim
next_word - copy and return, without delim

*/


Delimiter* init_word_delimiter(char* line) {
    return init_delimiter(line, " ", true);
}


int next_word(Delimiter* dl, char* dest) {
    // copy next word in dest, if present
    // returns true if word is found, false otherwise

    return next_token(dl, dest);
}

