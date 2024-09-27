#include<stdio.h>
#include<stdlib.h>
#include "parser.h"
#include "cstring.h"
#include "arithmetic.h"
#include "push.h"
#include "pop.h"


void get_file_variable_name(char* file_name, char* fvar) {
    // takes in full file name, i.e. a/b/c.ext and returns c

    char temp[128];
    copy_str(temp, file_name);

    int s_idx = 0;

    // remove all '/'
    int idx = search_char(temp, '/');
    while(idx!=-1) {
        s_idx += idx+1;
        idx = search_char(&temp[s_idx], '/');
        //printf("%d:%d:%s\n", idx, s_idx, &temp[idx+1]);
    }

    int max = search_char(&temp[s_idx], '.');
    copy_str_until(fvar, &temp[s_idx], max);
}


void parse_and_generate_asm(char* vc, FILE* asmFile, int* line_num, char* file_name) {

    int vcType = parse_virtual_command(vc);
    //printf("vcType=%d\n", vcType);

    if(vcType==Empty)
        // Empty string or comment
        return;

    *line_num = 1 + *line_num;

    // print virtual machine command for debugging
    fprintf(asmFile, "//%s\n", vc);
    char fvar[128];
    get_file_variable_name(file_name, fvar);

    //printf("fvar=%s;file_name=%s;vc=%s\n", fvar, file_name, vc);

    if(vcType == Push)
        handle_push(&vc[5], asmFile, fvar);
    else if(vcType == Pop)
        handle_pop(&vc[4], asmFile, fvar);

    else if(vcType == Arithmetic)
        handle_arithmetic_op(vc, asmFile);
    else if(vcType == Comparison)
        handle_comparison_op(vc, asmFile, line_num);
}

