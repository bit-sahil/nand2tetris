
#ifndef WRITER_H
#define WRITER_H


void add_bootstrap_code(FILE* asmFile);
    // add bootstrap code at beginning of assembly code

void parse_and_generate_asm(char* vc, FILE* asmFile, int* line_num, char* file_name, char* fvar);

#endif

