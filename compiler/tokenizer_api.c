#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../common/boolean.h"
#include "tokenizer.h"
#include "lookahead_tokenizer.h"


TokenizerConfig* init_tokenizer(char* file_name, char* out_ext) {
    // initialize tokenizer with relevant state to process files, 
    // to be able to return one token at a time

    TokenizerConfig* tc = (TokenizerConfig*) malloc(sizeof(TokenizerConfig));

    tc->dc = init_dir_config(file_name, ".jack", out_ext);
    tc->rc = NULL;
    tc->dl = NULL;

    // it's not a lookahead tokenizer
    tc->n = 0;

    return tc;
}


void dealloc_tokenizer(TokenizerConfig* tc) {
    // deallocate space taken by DirConfig and opened write file
    
    if(is_la_tokenizer(tc)) {
        free(tc->mem);
        free(tc->ttype);
    }

    dealloc_dir_config(tc->dc, NULL);

    if(tc->rc != NULL)
        dealloc_reader_config(tc->rc);

    if(tc->dl != NULL)
        dealloc_delimiter(tc->dl);

    free(tc);
}



TokenizerConfig* init_lookahead_tokenizer(char* file_name, char* out_ext, int max_lookahead) {
    // initialize tokenizer with relevant state to process files, 
    // to be able to return one token at a time

    TokenizerConfig* tc = init_tokenizer(file_name, out_ext);

    if(max_lookahead > 0) {
        tc->n = max_lookahead + 1;
        tc->curr = -1;
        tc->idx = 0;

        tc->mem = (char*) malloc(WORD_SIZE*(tc->n)*sizeof(char));
        tc->ttype = (TokenType*) malloc(tc->n*sizeof(int));
    }

    return tc;
}


int has_more_tokens(TokenizerConfig* tc) {
    // returns true if more tokens are there to be processed, ow false
    // if there's a non-empty word, that means there should be a token

    if(is_la_tokenizer(tc))
        return has_more_tokens_la(tc);

    return has_more_tokens_nla(tc);
}


void advance_token(TokenizerConfig* tc) {
    // advance token and store it's result to be retrieved later

    if(is_la_tokenizer(tc))
        return advance_token_la(tc);

    return advance_token_nla(tc);
}


TokenType get_token_type(TokenizerConfig* tc) {
    // return current token

    if(is_la_tokenizer(tc))
        return get_token_type_la(tc);

    return get_token_type_nla(tc);
}


const char* get_raw_token(TokenizerConfig* tc) {
    // returns raw token value as a string
    // this value should not be updated by caller so trying const there

    if(is_la_tokenizer(tc))
        return get_raw_token_la(tc);

    return get_raw_token_nla(tc);
}


char get_symbol_token(char* raw) {
    // returns symbol char from raw string

    return raw[0];
}


char get_symbol(TokenizerConfig* tc) {
    // returns symbol char

    if(get_token_type(tc) != SYMBOL)
        printf("TokenizerAPIError: Incorrect method (SYMBOL) called to retrieve current token for TokenType:%d, token:%s\n", tc->t, tc->token);

    return get_symbol_token(get_raw_token(tc));
}


const char* get_keyword(TokenizerConfig* tc) {
    // returns pointer to stored keyword string

    if(get_token_type(tc) != KEYWORD)
        printf("TokenizerAPIError: Incorrect method (KEYWORD) called to retrieve current token for TokenType:%d, token:%s\n", tc->t, tc->token);

    return get_raw_token(tc);
}


const char* get_identifier(TokenizerConfig* tc) {
    // returns pointer to identifier (string)

    if(get_token_type(tc) != IDENTIFIER)
        printf("TokenizerAPIError: Incorrect method (IDENTIFIER) called to retrieve current token for TokenType:%d, token:%s\n", tc->t, tc->token);

    return get_raw_token(tc);
}


long get_int_val_token(char* raw) {
    // returns integer value of a string

    char* err;

    long val = strtol(raw, &err, 10);

    if(err[0] != '\0')
        printf("TokenizerAPIError: while converting token to number, token=%s; err=%s\n", raw, err);

    return val;
}


long get_int_val(TokenizerConfig* tc) {
    // returns integer value

    if(get_token_type(tc) != INT_CONST)
        printf("TokenizerAPIError: Incorrect method (INT_CONST) called to retrieve current token for TokenType:%d, token:%s\n", tc->t, tc->token);

    return get_int_val_token(get_raw_token(tc));
}


char* get_string_val(TokenizerConfig* tc) {
    // returns pointer to string constant

    if(get_token_type(tc) != STRING_CONST)
        printf("TokenizerAPIError: Incorrect method (STR_CONST) called to retrieve current token for TokenType:%d, token:%s\n", tc->t, tc->token);

    return get_raw_token(tc);
}

