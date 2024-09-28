
#ifndef GOTO_H
#define GOTO_H


void handle_label(char* label, FILE* asmFile, char* fvar);
    // form:<label>

void handle_goto(char* label, FILE* asmFile, char* fvar);
    // form:<label>

void handle_if_goto(char* label, FILE* asmFile, char* fvar);
    // form:<label>

#endif

