#include <stdio.h>
#include "cstring.h"


void print_str(char* str) {
    // just prints a string. %s was printing random characters so resorted to a basic function

    int idx = 0;
    while(str[idx] != '\0') {
        printf("%c", str[idx]);
        // if(str[idx] == '\n') break;
        // handling '\n' shouldn't be required
        idx++;
    }
}


int compare_str(char* s1, char* s2) {
    // compare string character by character

    int i =0;
    // printf("%s;%s\n", s1, s2);
    while(s1[i] == s2[i]) {
        if(s1[i] == '\0') return True;
        i++;
    }
    return False;
}


void copy_str(char* dest, char* src) {
    // assume destination has required space and copy source string characters
    // strcpy was possibly copying erronously and hence this function (at least printing is erronous)

    int i = 0;
    while(1) {
        dest[i] = src[i];
        if(src[i] == '\0') return;
        i++;
    }
}


void copy_str_until(char* dest, char* src, int max) {
    // assume destination has required space and copy source string characters
    // copy until end of src string or max number of characters (excluding \0)
    // if max=-1, copy entire string

    if(max==-1) {
        copy_str(dest, src);
        return;
    }

    int i = 0;
    while(i<max && src[1]!='\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}


int search_char(char* str, char c) {
    // search for c in str and returns it's index
    // return -1 if not found

    int i = 0;
    while(str[i++] != '\0')
        if(str[i] == c)
            return i;
    
    return -1;
}


int find_substr(char* original, char* sub) {
    // this function find first occurence of sub in original string
    
    if(sub[0] == '\0') return 0;  // all substrings are valid for empty substring
    
    int idx = 0, found = False;

    while(original[idx] != '\0') {
        int i = 0;
        found = True;
        while(sub[i] != '\0') {
            if(original[idx+i]=='\0')
                return -1; //original string ends before substring could be found

            if(original[idx+i] != sub[i]) {
                found=False;
                break;
            }
            i++;
        }
        if(found) return idx;;
        idx++;
    }
    
    return -1;
}


void replace_substr_end(char* original, char* replacement, int idx) {
    // this function replaces part of original string starting at idx (for idx>0) and in the end for idx=-1
    
    if(idx==-1) {
        idx = 0;
        while(original[idx] != '\0') idx++;
    }

    int i = 0;
    while(replacement[i] != '\0') {
        original[idx+i] = replacement[i];
        i++;
    }
    original[idx+i] = '\0';
}

