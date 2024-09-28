#include<stdio.h>
#include<stdlib.h>
#include "cstring.h"
#include "helper.h"


void get_label(char* label, char* fvar, char* f_label) {
    // f_label = fvar$label

    copy_str(f_label, fvar);
    replace_substr_end(f_label, "$", -1);
    replace_substr_end(f_label, label, -1);
}
 

void handle_label(char* label, FILE* asmFile, char* fvar) {
    // form:<label>

    char f_label[128];
    get_label(label, fvar, f_label);

    fprintf(asmFile, "(%s)\n", f_label);
}


void handle_goto(char* label, FILE* asmFile, char* fvar) {
    // form:<label>

    char* f_contents = get_f_contents("goto");

    char f_label[128];
    get_label(label, fvar, f_label);

    // add file contents fortmatting label
    fprintf(asmFile, f_contents, f_label);

    free(f_contents);
}


void handle_if_goto(char* label, FILE* asmFile, char* fvar) {
    // form:<label>

    char* f_contents = get_f_contents("if_goto");

    char f_label[128];
    get_label(label, fvar, f_label);

    // add file contents fortmatting label
    fprintf(asmFile, f_contents, f_label);

    free(f_contents);
}

