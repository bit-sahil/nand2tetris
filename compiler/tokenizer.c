#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "../common/boolean.h"
#include "../common/file_handler.h"
#include "../common/file_reader.h"
#include "../common/code_reader.h"
#include "../common/word_delimit.h"
#include "tokenizer.h"
#include "token_handler.h"


TokenizerConfig* init_tokenizer(char* file_name, char* out_ext) {
    // initialize tokenizer with relevant state to process files, 
    // to be able to return one token at a time

    TokenizerConfig* tc = (TokenizerConfig*) malloc(sizeof(TokenizerConfig));

    tc->dc = init_dir_config(file_name, ".jack", out_ext);
    tc->rc = NULL;
    tc->dl = NULL;

    return tc;
}


void dealloc_tokenizer(TokenizerConfig* tc) {
    // deallocate space taken by DirConfig and opened write file
    dealloc_dir_config(tc->dc, NULL);

    if(tc->rc != NULL)
        dealloc_reader_config(tc->rc);

    if(tc->dl != NULL)
        dealloc_delimiter(tc->dl);

    free(tc);
}


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

    tc->dl = init_word_delimiter(l);

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

    while(!next_word(tc->dl, word)) {
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

    tc->state = CODE;
}


int has_more_tokens(TokenizerConfig* tc) {
    // returns true if more tokens are there to be processed, ow false
    // if there's a non-empty word, that means there should be a token

    return (tc->wp != NULL);
}


void advance_token(TokenizerConfig* tc) {
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
        if(handle_keyword(tc)) {
            tc->t = KEYWORD;
        } else {
            handle_identifier(tc);
            tc->t = IDENTIFIER;
        }
    }

    // now we need to check whether word has ended and handle that
    if(*tc->wp == '\0')
        if(!adv_next_word(tc))
            tc->wp = NULL;
}


TokenType get_token_type(TokenizerConfig* tc) {
    // return current token

    return tc->t;
}


char get_symbol(TokenizerConfig* tc) {
    // returns symbol char

    if(tc->t != SYMBOL)
        printf("Error: Incorrect method (SYMBOL) called to retrieve current token for TokenType:%d, token:%s\n", tc->t, tc->token);

    return tc->token[0];
}


char* get_keyword(TokenizerConfig* tc) {
    // returns pointer to stored keyword string

    if(tc->t != KEYWORD)
        printf("Error: Incorrect method (KEYWORD) called to retrieve current token for TokenType:%d, token:%s\n", tc->t, tc->token);

    return tc->token;
}


char* get_identifier(TokenizerConfig* tc) {
    // returns pointer to identifier (string)

    if(tc->t != IDENTIFIER)
        printf("Error: Incorrect method (IDENTIFIER) called to retrieve current token for TokenType:%d, token:%s\n", tc->t, tc->token);

    return tc->token;
}


long get_int_val(TokenizerConfig* tc) {
    // returns integer value

    if(tc->t != INT_CONST)
        printf("Error: Incorrect method (INT_CONST) called to retrieve current token for TokenType:%d, token:%s\n", tc->t, tc->token);

    char* err;

    long val = strtol(tc->token, &err, 10);

    if(err[0] != '\0')
        printf("Error while converting to number, token=%s; err=%s\n", tc->token, err);

    return val;
}


char* get_string_val(TokenizerConfig* tc) {
    // returns pointer to string constant

    if(tc->t != STRING_CONST)
        printf("Error: Incorrect method (STR_CONST) called to retrieve current token for TokenType:%d, token:%s\n", tc->t, tc->token);

    return tc->token;
}


void out_str(char* token, char* tokenType, FILE* outfp) {
        fprintf(outfp, "<%s> %s </%s>\n", tokenType, token, tokenType);
}


void out_sym_str(char* token, char* tokenType, FILE* outfp) {
    // output to file, also handling symbols for xm
    fprintf(outfp, "<%s> ", tokenType);

    char curr;
    for(int i=0; (token[i] != '\0'); i++) {

        switch(token[i]) {
            case '<':
                fprintf(outfp, "&lt;");
                break;
            case '>':
                fprintf(outfp, "&gt;");
                break;
            case '"':
                fprintf(outfp, "&quot;");
                break;
            case '&':
                fprintf(outfp, "&amp;");
                break;
            default:
                fprintf(outfp, "%c", token[i]);
        }
    }

    fprintf(outfp, " </%s>\n", tokenType);
}


void out_token(TokenizerConfig* tc, FILE* outfp) {
    // add token into xml file

    switch(tc->t) {

        case SYMBOL:
            out_sym_str(tc->token, "symbol", outfp);
            break;

        case KEYWORD:
            out_str(tc->token, "keyword", outfp);
            break;

        case IDENTIFIER:
            out_str(tc->token, "identifier", outfp);
            break;

        case INT_CONST:
            out_str(tc->token, "integerConstant", outfp);
            break;

        case STRING_CONST:
            out_sym_str(tc->token, "stringConstant", outfp);
            break;
    }
}


void generate_output(TokenizerConfig* tc) {
    // create output file corresponding to each input file
    FILE* outfp = get_out_file(tc->dc, tc->rc->file_name);

    fprintf(outfp, "<tokens>\n");

    while(has_more_tokens(tc)) {
        advance_token(tc);
        //printf("word:%s\n", tc->word);
        printf("token:%s;type=%d\n", tc->token, tc->t);
        out_token(tc, outfp);
    }

    fprintf(outfp, "</tokens>\n");
    fclose(outfp);
}


void handle_file_or_directory(char* file_name) {
    TokenizerConfig* tc = init_tokenizer(file_name, "Tout.xml");

    while(adv_next_file(tc)) {
        // file should be returned first time
        // printf("Error: No file returned for the first time: file_name=%s\n", file_name);

        generate_output(tc);
    }

    dealloc_tokenizer(tc);
}


int main(int argc, char* argv[]) {
    // .jack file or directory name is provided as argv[1]

    char* file_name = argv[1];

    handle_file_or_directory(file_name);
}

