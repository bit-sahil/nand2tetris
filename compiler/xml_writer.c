#include<stdio.h>
#include "tokenizer.h"
#include "tokenizer_api.h"
#include "config.h"


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


void out_symbol(char c, char* tokenType, FILE* outfp) {
    // output to file, also handling symbols for xml
    fprintf(outfp, "<%s> ", tokenType);

    switch(c) {
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
            fprintf(outfp, "%c", c);
    }

    fprintf(outfp, " </%s>\n", tokenType);
}


void begin(char* token, FILE* outfp) {
    fprintf(outfp, "<%s>\n", token);
}


void end(char* token, FILE* outfp) {
    fprintf(outfp, "</%s>\n", token);
}


void terminal(char* token_value, int action, FILE* outfp) {
    if(action == KEYWORD) {
        out_str(token_value, "keyword", outfp);
    } else if (action == SYMBOL) {
        char c = get_symbol_token(token_value);
        out_symbol(c, "symbol", outfp);
    } else if (action == IDENTIFIER) {
        out_str(token_value, "identifier", outfp);
    } else if (action == INT_CONST) {
        out_str(token_value, "integerConstant", outfp);
    } else if (action == STRING_CONST) {
        out_sym_str(token_value, "stringConstant", outfp);
    }
}


void out_xml(char* token, int action, GenConfig* genConfig) {
    // printf("token=%s;action=%d\n", token, action);
    
    if(action == BEGIN) {
        begin(token, genConfig->outfp);
    } else if (action == END) {
        end(token, genConfig->outfp);
    } else {
        // code to output terminals
        // token contains value and action is among TokenType
        terminal(token, action, genConfig->outfp);
    }
}
