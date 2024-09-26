
#ifndef PARSER_H
#define PARSER_H


// VM command type
#define Empty 0
#define Push 1
#define Pop 2
#define Add 3
#define Sub 4
#define Neg 5
#define Eq 6
#define Gt 7
#define Lt 8
#define And 9
#define Or 10
#define Not 11
#define Arithmetic 12
#define Comparison 13

// Stack segment types
#define Argument 101
#define Local 102
#define Static 103
#define Constant 104
#define This 105
#define That 106
#define Pointer 107
#define Temp 108


int parse_numeric_arg(char* arg);
    // parse through argument of command to retrieve characters corresponding to numeric argument
    // convert to integer and return

int parse_stack_segment(char* seg);
    // Parse through argument of push or pop command to determine segment type

int parse_virtual_command(char* vc);

#endif

