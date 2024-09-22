
#ifndef CSTRING_H
#define CSTRING_H


#define True 1
#define False 0


void print_str(char* str);

int compare_str(char* s1, char* s2);

void copy_str(char* dest, char* src);

void copy_str_until(char* dest, char* src, int max);
    // assume destination has required space and copy source string characters
    // copy until end of src string or max number of characters (excluding \0)

int search_char(char* str, char c);

int find_substr(char* original, char* sub);

void replace_substr_end(char* original, char* replacement, int idx);

#endif

