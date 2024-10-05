
#ifndef TOKEN_HANDLER_H
#define TOKEN_HANDLER_H


#include "tokenizer.h"


int handle_symbol(char curr);
    // return symbol if match, otherwise -1

void handle_digit(TokenizerConfig* tc);
    // store digit until it ends

void handle_str_const(TokenizerConfig* tc);
    // store string constant without "

int handle_keyword_token(TokenizerConfig* tc);
    // store keyword in token field if keyword is found, and return True
    // return False if word doesn't match keyword

void handle_identifier_token(TokenizerConfig* tc);
    // store identifier/variable name
    // alphanumeric and _ are applicable

#endif

