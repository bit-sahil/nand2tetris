#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "../common/boolean.h"
#include "../common/file_handler.h"
#include "../common/file_reader.h"
#include "../common/code_reader.h"
#include "../common/delimiter.h"
#include "../common/word_delimit.h"
#include "tokenizer.h"


int handle_symbol(char curr) {
    // return symbol if match, otherwise -1

    switch(curr) {
        case '{':
            return '{';
        case '}':
            return '}';
        case '(':
            return '(';
        case ')':
            return ')';
        case '[':
            return '[';
        case ']':
            return ']';
        case '.':
            return '.';
        case ',':
            return ',';
        case ';':
            return ';';
        case '+':
            return '+';
        case '-':
            return '-';
        case '*':
            return '*';
        case '/':
            return '/';
        case '&':
            return '&';
        case '|':
            return '|';
        case '<':
            return '<';
        case '>':
            return '>';
        case '=':
            return '=';
        case '~':
            return '~';
    }

    //printf("Unhandled char:%c\n", curr);

    return -1;
}


void handle_digit(TokenizerConfig* tc) {
    // store digit until it ends

    int idx=0;

    while(isdigit(*tc->wp)) {
        tc->token[idx++] = *tc->wp;
        tc->wp++;
    }

    tc->token[idx] = '\0';
}


void handle_str_const(TokenizerConfig* tc) {
    // store string constant without "

    tc->wp++;

    for(int idx=0; ; idx++) {

        if(*tc->wp == '\0') {
            // line termination without end of string
            printf("Error: line termination with End of String:%s\n", tc->rc->line);
            return;
        }

        //printf("curr=%c;prev=%c;cond=%d\n", *(tc->wp), *(tc->wp - 1), !(*(tc->wp - 1) != '\\'));

        if((*(tc->wp) == '"') && (*(tc->wp - 1) != '\\')) {
            // string termination
            //printf("Terminating string now\n");
            tc->token[idx] = '\0';
            tc->wp++;
            return;
        } else {
            // part of string constant
            tc->token[idx] = *tc->wp;
            tc->wp++;
        }
    }
}


int handle_keyword(TokenizerConfig* tc) {
    // store keyword in token field if keyword is found, and return True
    // return False if word doesn't match keyword

    char* keywords = "class|constructor|function|method|field|static|var|int|char|boolean|void|true|false|null|this|let|do|if|else|while|return";

    Delimiter* kw_dl = init_delimiter(keywords, "|", False);
    char kw[32];
    int len;

    //printf("word:%s;wp=%s\n", tc->word, tc->wp);

    while(next_token(kw_dl, kw)) {
        len = strlen(kw);

        if(strncmp(tc->wp, kw, len) == 0) {

            // handle cases where keyword is beginning of an identifier, i.e. var1
            if(isalnum(tc->wp[len]) || tc->wp[len]=='_')
                continue;

            strncpy(tc->token, tc->wp, len);
            tc->token[len] = '\0';
            tc->wp = tc->wp + len;
            dealloc_delimiter(kw_dl);
            return True;
        }
    }

    dealloc_delimiter(kw_dl);
    return False;
}


void handle_identifier(TokenizerConfig* tc) {
    // store identifier/variable name
    // alphanumeric and _ are applicable

    int idx=0;

    //printf("identifier;word:%s;wp=%s\n", tc->word, tc->wp);

    while(isalnum(tc->wp[idx]) || tc->wp[idx]=='_') {
        tc->token[idx] = tc->wp[idx];
        idx++;
    }

    //printf("wp:%c\n", *tc->wp);

    tc->token[idx] = '\0';
    tc->wp = tc->wp + idx;
}

