
#ifndef TOKENIZER_API_H
#define TOKENIZER_API_H


#include "tokenizer.h"


TokenizerConfig* init_tokenizer(char* file_name, char* out_ext);
    // initialize tokenizer with relevant state to process files, 
    // to be able to return one token at a time


void dealloc_tokenizer(TokenizerConfig* tc);
    // deallocate space taken by DirConfig and opened write file


TokenizerConfig* init_lookahead_tokenizer(char* file_name, char* out_ext, int max_lookahead);
    // initialize tokenizer with relevant state to process files, 
    // to be able to return one token at a time


int has_more_tokens(TokenizerConfig* tc);
    // returns true if more tokens are there to be processed, ow false
    // if there's a non-empty word, that means there should be a token


void advance_token(TokenizerConfig* tc);
    // advance token and store it's result to be retrieved later


TokenType get_token_type(TokenizerConfig* tc);
    // return current token


char* get_raw_token(TokenizerConfig* tc);
    // returns raw token value as a string
    // this value should not be updated by caller so trying const there


char get_symbol_token(char* raw);
    // returns symbol char from raw string


char get_symbol(TokenizerConfig* tc);
    // returns symbol char


char* get_keyword(TokenizerConfig* tc);
    // returns pointer to stored keyword string


char* get_identifier(TokenizerConfig* tc);
    // returns pointer to identifier (string)


long get_int_val_token(char* raw);
    // returns integer value of a string


long get_int_val(TokenizerConfig* tc);
    // returns integer value


char* get_string_val(TokenizerConfig* tc);
    // returns pointer to string constant


#endif
