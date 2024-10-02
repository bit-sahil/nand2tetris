
#ifndef TOKERNIZER_H
#define TOKERNIZER_H


#include "../common/file_handler.h"
#include "../common/file_reader.h"
#include "../common/code_reader.h"
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


/*
typedef enum {
    CLASS, 
    METHOD, 
    FUNCTION,
    CONSTRUCTOR, 
    INT,
    BOOLEAN, 
    CHAR, 
    VOID,
    VAR, 
    STATIC, 
    FIELD, 
    LET,
    DO, 
    IF, 
    ELSE, 
    WHILE,
    RETURN, 
    TRUE, 
    FALSE,
    NULL, 
    THIS,
} Keyword;
*/


typedef struct TokenizerConfig {
    DirConfig* dc;
    ReaderConfig* rc;
    Delimiter* dl;

    char word[WORD_SIZE], *wp;  // pointer to curr idx in word
    char token[WORD_SIZE];
    TokenType t;
} TokenizerConfig;


TokenizerConfig* init_tokenizer(char* file_name, char* out_ext);
    // initialize tokenizer with relevant state to process files, 
    // to be able to return one token at a time

int adv_next_file(TokenizerConfig* tc);
    /* advance to next file and update tc to store it
    If there are no more files to process, return false, o.w. return true
    */

int adv_next_line(TokenizerConfig* tc);
    /* advance to next line and update tc to store it
    if it's end of file, return false, o.w. return true
    */

int adv_next_word(TokenizerConfig* tc);
    /* next word is a string delimited by spaces
    this could itself be a bunch of tokens, i.e. word = "{i=197+3};"
    But until we have a word, not parsed until last character
    Since we've already gotten rid of all comments, it should mean we do have more tokens

    next_word() function name is already taken up, so adv_next_word() and so on.. for other functions too
    */

int has_more_tokens(TokenizerConfig* tc);
    // returns true if more tokens are there to be processed, ow false
    // if there's a non-empty word, that means there should be a token

void advance_token(TokenizerConfig* tc);
    // advance token and store it's result to be retrieved later

TokenType get_token_type(TokenizerConfig* tc);
    // return current token

char get_symbol(TokenizerConfig* tc);
    // returns symbol char

char* get_keyword(TokenizerConfig* tc);
    // returns pointer to stored keyword string

char* get_identifier(TokenizerConfig* tc);
    // returns pointer to identifier (string)

long get_int_val(TokenizerConfig* tc);
    // returns integer value

char* get_string_val(TokenizerConfig* tc);
    // returns pointer to string constant

int init_advance(TokenizerConfig* tc);

#endif

