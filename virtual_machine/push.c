#include<stdio.h>
#include<stdlib.h>
#include "parser.h"
#include "cstring.h"
#include "helper.h"


void handle_push_constant(int num, FILE* asmFile) {
    // push constant i pushed i onto stack

    char* f_contents = get_f_contents("push_constant");

    // add file contents after formatting with num
    fprintf(asmFile, f_contents, num);

    free(f_contents);
}


void handle_push_heap_segment(char* segment, int num, FILE* asmFile) {
    // Handles local, argument, this and that heap segments
    // push segment i accessed as segment+i, and pushes value at segment+i onto stack

    char* f_contents = get_f_contents("push_segment");

    // add file contents fortmatting num and segment
    fprintf(asmFile, f_contents, num, segment);

    free(f_contents);
}


void handle_push_temp(int num, FILE* asmFile) {
    // Handles temp heap segments
    // push temp i accessed as 5+i, but i can range only through 0..7

    if(num<0 || num>=8)
        // just raise a warning without stopping
        printf("Invalid access of temp location: %d\n", num);

    char* f_contents = get_f_contents("push_temp");

    // add file contents fortmatting num and segment, using R5 for Ram[5]
    fprintf(asmFile, f_contents, num, "R5");

    free(f_contents);
}


void handle_push_pointer(int num, FILE* asmFile) {
    // Handles pointer heap segments
    // push pointer i accesses this and that, 3+i, where i could only be 0 or 1

    if(num<0 || num>=2)
        // just raise a warning without stopping
        printf("Invalid access of pointer location: %d\n", num);

    char* f_contents = get_f_contents("push_temp");

    // add file contents fortmatting num and segment
    fprintf(asmFile, f_contents, num, "R3");

    free(f_contents);
}


void handle_push(char* seg, FILE* asmFile) {
    // form:<segment> num

    int segType = parse_stack_segment(seg);

    int idx = search_char(seg, ' ');
    char segment[9];
    copy_str_until(segment, seg, idx);
    int num = parse_numeric_arg(&seg[idx+1]);

    //printf("handle_push;type=%s;%d;%s;num=%d\n", seg, segType, segment, num);

    if(segType==Constant) {
        handle_push_constant(num, asmFile);
    } else if(segType==Local) {
        handle_push_heap_segment("LCL", num, asmFile);
    } else if(segType==Argument) {
        handle_push_heap_segment("ARG", num, asmFile);
    } else if(segType==This) {
        handle_push_heap_segment("THIS", num, asmFile);
    } else if(segType==That) {
        handle_push_heap_segment("THAT", num, asmFile);
    } else if(segType==Temp) {
        handle_push_temp(num, asmFile);
    } else if(segType==Pointer) {
        handle_push_pointer(num, asmFile);
    }
}

