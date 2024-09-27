#include<stdio.h>
#include<stdlib.h>
#include "parser.h"
#include "cstring.h"
#include "arithmetic.h"
#include "push.h"


void parse_and_generate_asm(char* vc, FILE* asmFile, int* line_num) {

    int vcType = parse_virtual_command(vc);
    //printf("vcType=%d\n", vcType);

    if(vcType==Empty)
        // Empty string or comment
        return;

    *line_num = 1 + *line_num;

    // print virtual machine command for debugging
    fprintf(asmFile, "//%s\n", vc);

    if(vcType == Push)
        handle_push(&vc[5], asmFile);

    else if(vcType == Arithmetic)
        handle_arithmetic_op(vc, asmFile);
    else if(vcType == Comparison)
        handle_comparison_op(vc, asmFile, line_num);
}

