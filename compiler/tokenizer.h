
#ifndef TOKERNIZER_H
#define TOKERNIZER_H


#include "../common/file_handler.h"
#include "../common/file_reader.h"
#include "../common/word_delimit.h"


#define LINE_SIZE 1024
#define WORD_SIZE 128


typedef enum {
    KEYWORD, 
    SYMBOL,
    IDENTIFIER, 
    INT_CONST,
    STRING_CONST,
} TokenType;


#define BEGIN 10
#define END 11


typedef struct TokenizerConfig {
    DirConfig* dc;
    ReaderConfig* rc;
    Delimiter* dl;

    char word[WORD_SIZE], *wp;  // pointer to curr idx in word
    char token[WORD_SIZE];
    TokenType t;

    // Specific to lookahead tokenizer
    // memory is allocated at runtime so not much overhead
    int n, curr, idx;
    char* mem;
    TokenType* ttype;
} TokenizerConfig;


int adv_next_file(TokenizerConfig* tc);
    /* advance to next file and update tc to store it
    If there are no more files to process, return false, o.w. return true
 
    Also advances to first non-empty line, and word for tokenizer to process
    */

int init_advance(TokenizerConfig* tc);
    // seek to first non-empty line and word for tokens to start processing

int has_more_tokens_nla(TokenizerConfig* tc);
    // returns true if more tokens are there to be processed, ow false
    // if there's a non-empty word, that means there should be a token

void advance_token_nla(TokenizerConfig* tc);
    // advance token and store it's result to be retrieved later

TokenType get_token_type_nla(TokenizerConfig* tc);
    // return current token

char* get_raw_token_nla(TokenizerConfig* tc);
    // returns raw token value as a string

#endif
