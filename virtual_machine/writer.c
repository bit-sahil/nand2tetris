#include<stdio.h>
#include "parser.h"



void handle_push(char* seg, FILE* asmFile) {
    // form:<segment> num

    int segType = parse_stack_segment(seg);
    int num = -1;

    if(segType==Constant) {
        // constant num
        num = parse_numeric_arg(&seg[9]);
        //printf("constant segment:%s:%d\n", &seg[9], num);

        // write in asmFile
        fprintf(asmFile, "@%d\n", num);
        fputs("D=A\n", asmFile);
        fputs("@SP\n", asmFile);
        fputs("A=M\n", asmFile);
        fputs("M=D\n", asmFile);
        fputs("@SP\n", asmFile);
        fputs("M=M+1\n", asmFile);
    }
}


void handle_add(FILE* asmFile) {
    // pop two numbers from stack, store back result on stack

    fputs("@SP\n", asmFile);
    fputs("M=M-1\n", asmFile);
    fputs("A=M\n", asmFile);
    fputs("D=M\n", asmFile);
    fputs("@SP\n", asmFile);
    fputs("M=M-1\n", asmFile);
    fputs("A=M\n", asmFile);
    fputs("D=D+M\n", asmFile);
    fputs("M=D\n", asmFile);
    fputs("@SP\n", asmFile);
    fputs("M=M+1\n", asmFile);
}


void parse_and_generate_asm(char* vc, FILE* asmFile) {

    int vcType = parse_virtual_command(vc);
    //printf("%d\n", vcType);

    if(vcType==Empty)
        // Empty string or comment
        return;

    // print virtual machine command for debugging
    fprintf(asmFile, "//%s\n", vc);

    if(vcType == Push)
        handle_push(&vc[5], asmFile);

    else if(vcType == Add)
        handle_add(asmFile);
}

