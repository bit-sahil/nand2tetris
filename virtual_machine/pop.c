#include<stdio.h>
#include<stdlib.h>
#include "parser.h"
#include "cstring.h"
#include "helper.h"


void handle_pop_heap_segment(char* segment, int num, FILE* asmFile) {
    // Handles local, argument, this and that heap segments
    // pop top value of stack and pushes it onto segment i accessed as segment+i

    char* f_contents = get_f_contents("pop_segment");

    // add file contents fortmatting num and segment
    fprintf(asmFile, f_contents, num, segment);

    free(f_contents);
}


void handle_pop_temp(int num, FILE* asmFile) {
    // Handles temp heap segments
    // pop temp i accessed as 5+i, but i can range only through 0..7

    if(num<0 || num>=8)
        // just raise a warning without stopping
        printf("Invalid access of temp location: %d\n", num);

    char* f_contents = get_f_contents("pop_temp");

    // add file contents fortmatting num and segment, using R5 for Ram[5]
    fprintf(asmFile, f_contents, num, "R5");

    free(f_contents);
}


void handle_pop_pointer(int num, FILE* asmFile) {
    // Handles pointer heap segments
    // pop pointer i accesses this and that, 3+i, where i could only be 0 or 1

    if(num<0 || num>=2)
        // just raise a warning without stopping
        printf("Invalid access of pointer location: %d\n", num);

    char* f_contents = get_f_contents("pop_temp");

    // add file contents fortmatting num and segment
    fprintf(asmFile, f_contents, num, "R3");

    free(f_contents);
}


void handle_pop(char* seg, FILE* asmFile) {
    // form:<segment> num

    int segType = parse_stack_segment(seg);

    int idx = search_char(seg, ' ');
    char segment[9];
    copy_str_until(segment, seg, idx);
    int num = parse_numeric_arg(&seg[idx+1]);

    //printf("handle_pop;type=%s;%d;%s;num=%d\n", seg, segType, segment, num);

    if(segType==Local) {
        handle_pop_heap_segment("LCL", num, asmFile);
    } else if(segType==Argument) {
        handle_pop_heap_segment("ARG", num, asmFile);
    } else if(segType==This) {
        handle_pop_heap_segment("THIS", num, asmFile);
    } else if(segType==That) {
        handle_pop_heap_segment("THAT", num, asmFile);
    } else if(segType==Temp) {
        handle_pop_temp(num, asmFile);
    } else if(segType==Pointer) {
        handle_pop_pointer(num, asmFile);
    }
}

