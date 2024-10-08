
#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include "config.h"


typedef struct ParserConfig {
    TokenizerConfig* tc;
} ParserConfig;


ParserConfig* init_parser(char* file_name, char* out_ext, int max_lookahead);
    // initialize parser config 


void dealloc_parser(ParserConfig* pc);
    // deallocate space taken by DirConfig and opened write file


void parse_and_generate_output(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc));
    // create output file corresponding to each input file, one at a time
    // file should begin with a class, and each file has only one class

#endif
