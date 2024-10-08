
#ifndef XML_WRITER_H
#define XML_WRITER_H

#include<stdio.h>


#define BEGIN 10
#define END 11


void out_str(char* token, char* tokenType, FILE* outfp);

void out_sym_str(char* token, char* tokenType, FILE* outfp);
    // output to file, also handling symbols for xm

void out_symbol(char c, char* tokenType, FILE* outfp);
    // output to file, also handling symbols for xml

void out_xml(char* token, int action, FILE* outfp);

#endif
