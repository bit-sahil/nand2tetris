#include<stdio.h>
#include<stdlib.h>
#include "parser.h"
#include "cstring.h"
#include "helper.h"


int get_function_symbol(char* args, char* fvar, char* f_label) {
    // stores function symbol in f_label
    // returns index of next argument to be parsed (number of args in particular)
    // todo: No longer required, deprecate this

    char f_name[128];
    int idx = search_char(args, ' ');
    copy_str_until(f_name, args, idx);

    //copy_str(f_label, fvar);
    //replace_substr_end(f_label, ".", -1);
    //replace_substr_end(f_label, f_name, -1);

    return idx;
}


int get_func_name(char* args, char* f_name) {
    // returns function name in f_name
    // returns index of next argument to be parsed (number of args in particular)

    int idx = search_char(args, ' ');
    copy_str_until(f_name, args, idx);

    return idx;
}


int update_fvar(char* args, char* fvar) {
    // update fvar to take on <file_name>.<function_name> value
    // function names are already defined in this way, so we just need to update fvar
    // also return index of next argument to be parsed

    int idx = search_char(args, ' ');
    copy_str_until(fvar, args, idx);

    return idx;
}


void handle_function(char* args, FILE* asmFile, char* fvar) {
    // form:<f_name> num

    char f_name[128];
    int idx = get_func_name(args, f_name);
    //int idx = update_fvar(args, fvar);
    int num = parse_numeric_arg(&args[idx+1]);

    //printf("handle_function;type=%s;%s;num=%d\n", args, fvar, num);

    // add file contents fortmatting function label and num
    fprintf(asmFile, "(%s)\n", f_name);

    // push 0 n times
    char* f_contents = get_f_contents("function_push");
    while(num--) {
        fprintf(asmFile, f_contents, 0);
    }

    free(f_contents);
}

