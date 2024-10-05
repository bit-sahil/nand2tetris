#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "../common/boolean.h"
#include "../common/file_handler.h"
#include "../common/file_reader.h"
#include "../common/code_reader.h"
#include "../common/code_delimit.h"
#include "tokenizer.h"
#include "token_handler.h"
#include "xml_writer.h"


int adv_next_file(TokenizerConfig* tc) {
    /* advance to next file and update tc to store it
    If there are no more files to process, return false, o.w. return true
 
    Also advances to first non-empty line, and word for tokenizer to process
    */

    // deallocate memory of previous ReaderConfig, if present
    if(tc->rc != NULL) {
        dealloc_reader_config(tc->rc);
        tc->rc = NULL;
    }

    char f_name[128];
    if(!get_next_file(tc->dc, f_name)) {
        return False;
    }

    tc->rc = init_code_reader(f_name);

    init_advance(tc);

    return True;
}


int adv_next_line(TokenizerConfig* tc) {
    /* advance to next line and update tc to store it
    if it's end of file, return false, o.w. return true
    */

    char *l;

    if((l = get_next_line(tc->rc)) == NULL) {
        // no more lines to read in file
        return False;
    }

    if(tc->dl != NULL)
        dealloc_delimiter(tc->dl);
        tc->dl = NULL;

    tc->dl = init_code_delimiter(l);

    return True;
}


int adv_next_word(TokenizerConfig* tc) {
    /* next word is a string delimited by spaces
    this could itself be a bunch of tokens, i.e. word = "{i=197+3};"
    But until we have a word, not parsed until last character
    Since we've already gotten rid of all comments, it should mean we do have more tokens

    next_word() function name is already taken up, so adv_next_word() and so on.. for other functions too
    */
    
    char word[128];

    while(!next_code_token(tc->dl, word)) {
        if(!adv_next_line(tc))
            return False;
    }

    strcpy(tc->word, word);
    tc->wp = tc->word;

    return True;
}


int init_advance(TokenizerConfig* tc) {
    // seek to first non-empty line and word for tokens to start processing

    if(!adv_next_line(tc)) {
        // file is empty as far as our code goes -- should not be the case
        printf("Error: Empty file: f_name=%s\n", tc->dc->file_name);
    }

    if(!adv_next_word(tc)) {
        // not empty line should anyways have non-empty word, but putting this redundant check anyhow
        printf("Error: Empty word in line: f_name=%s; line=%s\n", tc->dc->file_name, tc->rc->line);
    }

    tc->dl->state = CODE;
}


int has_more_tokens_nla(TokenizerConfig* tc) {
    // returns true if more tokens are there to be processed, ow false
    // if there's a non-empty word, that means there should be a token

    return (tc->wp != NULL);
}


void advance_token_nla(TokenizerConfig* tc) {
    // advance token and store it's result to be retrieved later

    int idx = 0;
    char curr = *tc->wp;

    if(handle_symbol(curr) != -1) {
        tc->token[0] = curr;
        tc->token[1] = '\0';
        tc->wp++;
        tc->t = SYMBOL;

    } else if(isdigit(curr)) {
        handle_digit(tc);
        tc->t = INT_CONST;

    } else if(curr == '"') {
        handle_str_const(tc);
        tc->t = STRING_CONST;

    } else {
        if(handle_keyword_token(tc)) {
            tc->t = KEYWORD;
        } else {
            handle_identifier_token(tc);
            tc->t = IDENTIFIER;
        }
    }

    // now we need to check whether word has ended and handle that
    if(*tc->wp == '\0')
        if(!adv_next_word(tc))
            tc->wp = NULL;
}


TokenType get_token_type_nla(TokenizerConfig* tc) {
    // return current token

    return tc->t;
}


const char* get_raw_token_nla(TokenizerConfig* tc) {
    // returns raw token value as a string
    // this value should not be updated by caller so trying const there

    return tc->token;
}

