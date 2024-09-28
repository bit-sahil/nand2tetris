#include<stdio.h>
#include<stdlib.h>
#include "parser.h"
#include "cstring.h"
#include "helper.h"


int get_function_symbol(char* args, char* fvar, char* f_label) {
    // stores function symbol in f_label
    // returns index of next argument to be parsed (number of args in particular)

    char f_name[128];
    int idx = search_char(args, ' ');
    copy_str_until(f_name, args, idx);

    copy_str(f_label, fvar);
    replace_substr_end(f_label, ".", -1);
    replace_substr_end(f_label, f_name, -1);

    return idx;
}


void handle_function(char* args, FILE* asmFile, char* fvar) {
    // form:<f_name> num

    // f_label would be <file_name>.f_name
    char f_label[128];
    int idx = get_function_symbol(args, fvar, f_label);
    int num = parse_numeric_arg(&args[idx+1]);

    //printf("handle_function;type=%s;%s;num=%d\n", args, f_label, num);

    // add file contents fortmatting function label and num
    fprintf(asmFile, "(%s)\n", f_label);

    // push 0 n times
    char* f_contents = get_f_contents("function_push");
    while(num--) {
        fprintf(asmFile, f_contents, 0);
    }

    free(f_contents);
}

