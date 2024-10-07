#include<stdio.h>
#include<stdlib.h>
#include "parser.h"
#include "cstring.h"
#include "arithmetic.h"
#include "push.h"
#include "pop.h"
#include "helper.h"
#include "function.h"
#include "return.h"
#include "call.h"
#include "goto.h"


void add_bootstrap_code(FILE* asmFile) {
    // add bootstrap code at beginning of assembly code

    char* f_contents = get_f_contents("bootstrap");

    // add SP=256
    fprintf(asmFile, f_contents);

    free(f_contents);

    // add call Sys.init
    handle_call("Sys.init 0", asmFile, 5); // line number is actually not required
}


void parse_and_generate_asm(char* vc, FILE* asmFile, int* line_num, char* file_name, char* fvar) {

    int vcType = parse_virtual_command(vc);
    //printf("vcType=%d\n", vcType);

    if(vcType==Empty)
        // Empty string or comment
        return;

    // print virtual machine command for debugging
    fprintf(asmFile, "//%s\n", vc);
    //printf("fvar=%s;vc=%s\n", fvar, vc);

    if(vcType == Push)
        handle_push(&vc[5], asmFile, fvar);
    else if(vcType == Pop)
        handle_pop(&vc[4], asmFile, fvar);

    else if(vcType == Arithmetic)
        handle_arithmetic_op(vc, asmFile);
    else if(vcType == Comparison)
        handle_comparison_op(vc, asmFile, *line_num, fvar);

    // label and goto
    else if(vcType == Label)
        handle_label(&vc[6], asmFile, fvar);
    else if(vcType == Goto)
        handle_goto(&vc[5], asmFile, fvar);
    else if(vcType == If_goto)
        handle_if_goto(&vc[8], asmFile, fvar);
    
    // function, call and return
    else if(vcType == Function)
        handle_function(&vc[9], asmFile, fvar);
    else if(vcType == Return) {
        handle_return(asmFile);
        // fvar should no longer contain function name, just file name
        get_file_variable_name(file_name, fvar);
    } else if(vcType == Call)
        handle_call(&vc[5], asmFile, *line_num);
}

