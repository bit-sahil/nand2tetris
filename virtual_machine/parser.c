#include <stdio.h>
#include "cstring.h"
#include "parser.h"


int parse_numeric_arg(char* arg) {
    // parse through argument of command to retrieve characters corresponding to numeric argument
    // convert to integer and return

    int num = 0;
    int i=0, c;

    // not handling max number size
    while(arg[i] != '\0') {
        c = arg[i]-'0';

        if(c<0 || c>9) {
            printf("parse_numeric_arg invalid char: %c\n", arg[i]);
        }
        
        num = num*10;
        num += c;
        i++;
    }

    return num;
}


int parse_stack_segment(char* seg) {
    // Parse through argument of push or pop command to determine segment type

    if(compare_str_until(seg, "argument", 8))
        return Argument;
    if(compare_str_until(seg, "local", 5))
        return Local;
    if(compare_str_until(seg, "static", 6))
        return Static;
    if(compare_str_until(seg, "constant", 8))
        return Constant;
    if(compare_str_until(seg, "this", 4))
        return This;
    if(compare_str_until(seg, "that", 4))
        return That;
    if(compare_str_until(seg, "pointer", 7))
        return Pointer;
    if(compare_str_until(seg, "temp", 4))
        return Temp;
    
    printf("Unhandled stack segment:%s\n", seg);
}


int parse_virtual_command(char* vc) {
    /* Parse through a single line of command and determine type of command to be handled
    */

    if(vc[0] == '\0') {
        //empty line
        return Empty;
    }

    // stack operations
    if(compare_str_until(vc, "push", 4))
        return Push;
    else if(compare_str_until(vc, "pop", 3))
        return Pop;

    // arithmetic operations
    else if(compare_str_until(vc, "add", 3))
        return Arithmetic;
        //return Add;
    else if(compare_str_until(vc, "sub", 3))
        return Arithmetic;
        //return Sub;
    else if(compare_str_until(vc, "neg", 3))
        return Arithmetic;
        //return Neg;
    else if(compare_str_until(vc, "eq", 2))
        return Comparison;
        //return Eq;
    else if(compare_str_until(vc, "gt", 2))
        return Comparison;
        //return Gt;
    else if(compare_str_until(vc, "lt", 2))
        return Comparison;
        //return Lt;
    else if(compare_str_until(vc, "and", 3))
        return Arithmetic;
        //return And;
    else if(compare_str_until(vc, "or", 2))
        return Arithmetic;
        //return Or;
    else if(compare_str_until(vc, "not", 3))
        return Arithmetic;
        //return Not;

    // label and goto commands
    else if(compare_str_until(vc, "label", 3))
        return Label;
    else if(compare_str_until(vc, "goto", 3))
        return Goto;
    else if(compare_str_until(vc, "if-goto", 3))
        return If_goto;

    // function call and return
    else if(compare_str_until(vc, "function", 3))
        return Function;
    else if(compare_str_until(vc, "call", 3))
        return Call;
    else if(compare_str_until(vc, "return", 3))
        return Return;
    
    printf("Unhandled virtual command:%s\n", vc);
}

