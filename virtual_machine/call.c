#include<stdio.h>
#include<stdlib.h>
#include "parser.h"
#include "cstring.h"
#include "helper.h"


int get_f_name(char* args, char* f_name) {
    // returns function name in f_name
    // returns index of next argument to be parsed (number of args in particular)

    int idx = search_char(args, ' ');
    copy_str_until(f_name, args, idx);

    return idx;
}


int get_call_symbol(char* f_name, char* c_label) {
    // stores call return symbol in c_label

    copy_str(c_label, f_name);
    replace_substr_end(c_label, "$Ret.", -1);
}


void handle_call(char* args, FILE* asmFile, int line_num) {
    // form:<f_name> num
    // using line_num instead of counter, but should do the trick

    // c_label would be <file_name>.<f_name>$Ret.  and <num> would be passed separately
    char f_name[128], c_label[128];
    int idx = get_f_name(args, f_name);
    get_call_symbol(f_name, c_label);
    int num = parse_numeric_arg(&args[idx+1]);

    //printf("handle_call;type=%s;%s;num=%d\n", args, c_label, num);

    char* f_contents = get_f_contents("call");
    
    // we need to pass c_label, num, f_label, but f_label is composed of f_label and line_num
    fprintf(asmFile, f_contents, c_label, line_num, num, f_name, c_label, line_num);

    free(f_contents);
}

